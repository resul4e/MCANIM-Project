#include "Model.h"

#include "Rig.h"
#include "Joint.h"

#include <glad/glad.h>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/dual_quaternion.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <vector>
#include "DQB/dual_quat_cu.hpp" 

using namespace Tbx;

/**
 * Function to deform a mesh with dual quaternions.
 *
 * @note Originally this was CUDA code. Aside from this deformer function
 * which uses std::vector every other classes and methods should be readily
 * convertible to CUDA code just by adding __host__ __device__ flags before
 * their definitions.
 *
 * @param in_verts : vector of mesh vertices
 * @param in_normals : vector of mesh normals (same order as 'in_verts')
 * @param out_verts : deformed vertices with dual quaternions
 * @param out_normals : deformed normals with dual quaternions
 * @param dual_quat : list of dual quaternions transformations per joints
 * @param weights : list of influence weights for each vertex
 * @param joints_id : list of joints influence fore each vertex (same order as 'weights')
 */
void dual_quat_deformer(const std::vector<Point3>& in_verts,
	const std::vector<Vec3>& in_normals,
	std::vector<Vec3>& out_verts,
	std::vector<Vec3>& out_normals,
	const std::vector<Dual_quat_cu>& dual_quat,
	const std::vector< std::vector<float> >& weights,
	const std::vector< std::vector<int> >& joints_id)
{
	for (unsigned v = 0; v < in_verts.size(); ++v)
	{
		const int nb_joints = weights[v].size(); // Number of joints influencing vertex p

		// Init dual quaternion with first joint transformation
		int   k0 = -1;
		float w0 = 0.f;
		Dual_quat_cu dq_blend;
		Quat_cu q0;

		if (nb_joints != 0)
		{
			k0 = joints_id[v][0];
			w0 = weights[v][0];
		}
		else
			dq_blend = Dual_quat_cu::identity();

		if (k0 != -1) dq_blend = dual_quat[k0] * w0;

		int pivot = k0;

		q0 = dual_quat[pivot].rotation();
		// Look up the other joints influencing 'p' if any
		for (int j = 1; j < nb_joints; j++)
		{
			const int k = joints_id[v][j];
			float w = weights[v][j];
			const Dual_quat_cu& dq = (k == -1) ? Dual_quat_cu::identity() : dual_quat[k];

			if (dq.rotation().dot(q0) < 0.f)
				w *= -1.f;

			dq_blend = dq_blend + dq * w;
		}

		// Compute animated position
		Vec3 vi = dq_blend.transform(in_verts[v]).to_vec3();
		out_verts[v] = vi;
		// Compute animated normal
		out_normals[v] = dq_blend.rotate(in_normals[v]);
	}
}

void Mesh::UpdateVertices(const Rig& rig)
{
	glBindVertexArray(vao);

	std::vector<Dual_quat_cu> dquats;
	std::vector<std::vector<float>> weights(positions.size(), std::vector<float>());
	std::vector<std::vector<int>> jointsID(positions.size(), std::vector<int>());
	std::vector<Vec3> animatedPositions(positions.size(), Vec3(0));
	std::vector<Vec3> animatedNormals(normals.size(), Vec3(0));
	for (int i = 0; i < m_bones.size(); i++)
	{
		const Bone& bone = m_bones[i];
		std::shared_ptr<Joint> joint = rig.GetJoint(bone.m_name);
		if (joint == nullptr) continue;

		const glm::mat4& offsetMatrix = bone.m_offsetMatrix;
		glm::mat4 globalTransform = joint->GetGlobalTransform();
		glm::mat4 finalTransform = globalTransform * offsetMatrix;

		Transfo mat(finalTransform[0][0], finalTransform[1][0], finalTransform[2][0], finalTransform[3][0],
			finalTransform[0][1], finalTransform[1][1], finalTransform[2][1], finalTransform[3][1],
			finalTransform[0][2], finalTransform[1][2], finalTransform[2][2], finalTransform[3][2],
			finalTransform[0][3], finalTransform[1][3], finalTransform[2][3], finalTransform[3][3]);
		
		auto dquat = Dual_quat_cu(mat);
		dquats.push_back(dquat);

		for (const VertexWeight& vertexWeight : bone.m_weights)
		{
			const int& vIndex = vertexWeight.m_vertexIndex;
			const float& vWeight = vertexWeight.m_weight;
			//glm::vec4 v = glm::vec4(positions[vIndex], 1);
			//animatedPositions[vIndex] += glm::vec3(finalTransform * v) * vWeight;
			//animatedNormals[vIndex] += glm::mat3(finalTransform) * normals[vertexWeight.m_vertexIndex] * vWeight;

			weights[vIndex].push_back(vWeight);
			jointsID[vIndex].push_back(i);
		}
	}

	std::vector<Point3> in_verts(positions.size());
	std::vector<Vec3> in_norms(positions.size());
	for (int i = 0; i < positions.size(); i++)
	{
		in_verts[i] = Point3(positions[i].x, positions[i].y, positions[i].z);
		in_norms[i] = Vec3(normals[i].x, normals[i].y, normals[i].z);
	}

	dual_quat_deformer(in_verts, in_norms, animatedPositions, animatedNormals, dquats, weights, jointsID);

	std::vector<glm::vec3> linearPositions(faces.size() * 3);
	std::vector<glm::vec3> linearNormals(faces.size() * 3);
	std::vector<glm::vec2> linearTextureCoords(faces.size() * 3);

	for (int i = 0; i < faces.size(); i++)
	{
		Face& face = faces[i];
		for (int v = 0; v < 3; v++)
		{
			linearPositions[i * 3 + v] = glm::vec3(animatedPositions[face.indices[v]].x, animatedPositions[face.indices[v]].y, animatedPositions[face.indices[v]].z);
			linearNormals[i * 3 + v] = glm::vec3(animatedNormals[face.indices[v]].x, animatedNormals[face.indices[v]].y, animatedNormals[face.indices[v]].z);
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, pbo);
	glBufferData(GL_ARRAY_BUFFER, linearPositions.size() * sizeof(glm::vec3), linearPositions.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glBufferData(GL_ARRAY_BUFFER, linearNormals.size() * sizeof(glm::vec3), linearNormals.data(), GL_DYNAMIC_DRAW);
}

void Mesh::Upload()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	std::vector<glm::vec3> linearPositions(faces.size() * 3);
	std::vector<glm::vec3> linearNormals(faces.size() * 3);
	std::vector<glm::vec2> linearTextureCoords(faces.size() * 3);

	std::cout << faces.size() << std::endl;
	for (int i = 0; i < faces.size(); i++)
	{
		Face& face = faces[i];
		for (int v = 0; v < 3; v++)
		{
			linearPositions[i * 3 + v] = positions[face.indices[v]];
			linearNormals[i * 3 + v] = normals[face.indices[v]];

			if (texCoords.size() != 0) {
				linearTextureCoords[i * 3 + v] = texCoords[face.indices[v]];
			}
		}
	}

	glGenBuffers(1, &pbo);
	glBindBuffer(GL_ARRAY_BUFFER, pbo);
	glBufferData(GL_ARRAY_BUFFER, linearPositions.size() * sizeof(glm::vec3), linearPositions.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &nbo);
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glBufferData(GL_ARRAY_BUFFER, linearNormals.size() * sizeof(glm::vec3), linearNormals.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	if (texCoords.size() != 0) {
		glGenBuffers(1, &tbo);
		glBindBuffer(GL_ARRAY_BUFFER, tbo);
		glBufferData(GL_ARRAY_BUFFER, linearTextureCoords.size() * sizeof(glm::vec2), linearTextureCoords.data(), GL_DYNAMIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);
	}
}

void Model::Upload()
{
	for (Mesh& mesh : meshes)
	{
		mesh.Upload();
	}
}

void Model::UpdateVertices(const Rig& rig)
{
	for (Mesh& mesh : meshes)
	{
		mesh.UpdateVertices(rig);
	}
}
