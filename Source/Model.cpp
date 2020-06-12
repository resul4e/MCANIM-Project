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
#include "glm/ext.hpp"
#include "DQS.h"

//glm::quat Mat4ToQuat(glm::mat4 _m)
//{
//	// Compute trace of matrix 't'
//	float T = 1 + _m[0][0] + _m[1][1] + _m[2][2];
//
//	float S, X, Y, Z, W;
//
//	if (T > 0.00000001f) // to avoid large distortions!
//	{
//		S = sqrt(T) * 2.f;
//		X = (_m[2][1] - _m[1][2]) / S;
//		Y = (_m[0][2] - _m[2][0]) / S;
//		Z = (_m[1][0] - _m[0][1]) / S;
//		W = 0.25f * S;
//	}
//	else
//	{
//		if (_m[0][0] > _m[1][1] && _m[0][0] > _m[1][1])
//		{
//			// Column 0 :
//			S = sqrt(1.0f + _m[0][0] - _m[1][1] - _m[2][2]) * 2.f;
//			X = 0.25f * S;
//			Y = (_m[1][0] + _m[0][1]) / S;
//			Z = (_m[0][2] + _m[2][0]) / S;
//			W = (_m[2][1] - _m[1][2]) / S;
//		}
//		else if (_m[1][1] > _m[2][2])
//		{
//			// Column 1 :
//			S = sqrt(1.0f + _m[1][1] - _m[0][0] - _m[2][2]) * 2.f;
//			X = (_m[1][0] + _m[0][1]) / S;
//			Y = 0.25f * S;
//			Z = (_m[2][1] + _m[1][2]) / S;
//			W = (_m[0][2] - _m[2][0]) / S;
//		}
//		else
//		{   // Column 2 :
//			S = sqrt(1.0f + _m[2][2] - _m[0][0] - _m[1][1]) * 2.f;
//			X = (_m[0][2] + _m[2][0]) / S;
//			Y = (_m[2][1] + _m[1][2]) / S;
//			Z = 0.25f * S;
//			W = (_m[1][0] - _m[0][1]) / S;
//		}
//	}
//	
//	return glm::quat(W, -X, -Y, -Z);
//}
//
//glm::dualquat CreateDualQuat(const glm::quat& q, const glm::vec3& t)
//{
//	float w = -0.5f * (t.x * q.x + t.y * q.y + t.z * q.z);
//	float i = 0.5f * (t.x * q.w + t.y * q.z - t.z * q.y);
//	float j = 0.5f * (-t.x * q.z + t.y * q.w + t.z * q.x);
//	float k = 0.5f * (t.x * q.y - t.y * q.x + t.z * q.w);
//
//	return glm::dualquat(q, glm::quat(w, i, j, k));
//}
//

void Mesh::UpdateVertices(const Rig& rig)
{
	glBindVertexArray(vao);

	animatedPositions = std::vector<glm::vec3>(positions.size(), glm::vec3(0));
	animatedNormals = std::vector<glm::vec3>(normals.size(), glm::vec3(0));
	
	DQS dqs;
	dqs.Skin(rig, *this);
	
	//std::vector<glm::dualquat> dquats;
	//
	//
	//
	//
	//std::vector<std::vector<float>> weights(positions.size(), std::vector<float>());
	//std::vector<std::vector<int>> jointsID(positions.size(), std::vector<int>());
	//
	//for (int i = 0; i < m_bones.size(); i++)
	//{
	//	const Bone& bone = m_bones[i];
	//	std::shared_ptr<Joint> joint = rig.GetJoint(bone.m_name);
	//	if (joint == nullptr) continue;
	//
	//	const glm::mat4& offsetMatrix = bone.m_offsetMatrix;
	//	glm::mat4 globalTransform = joint->GetGlobalTransform();
	//	glm::mat4 finalTransform = globalTransform * offsetMatrix;
	//
	//	auto trans = Mat4ToQuat(finalTransform);
	//	auto dq = CreateDualQuat(trans, glm::vec3(finalTransform[3][0], finalTransform[3][1], finalTransform[3][2]));
	//	
	//	dquats.push_back(dq);
	//
	//	for (const VertexWeight& vertexWeight : bone.m_weights)
	//	{
	//		const int& vIndex = vertexWeight.m_vertexIndex;
	//		const float& vWeight = vertexWeight.m_weight;
	//		//glm::vec4 v = glm::vec4(positions[vIndex], 1);
	//		//animatedPositions[vIndex] += glm::vec3(finalTransform * v) * vWeight;
	//		//animatedNormals[vIndex] += glm::mat3(finalTransform) * normals[vertexWeight.m_vertexIndex] * vWeight;
	//
	//		weights[vIndex].push_back(vWeight);
	//		jointsID[vIndex].push_back(i);
	//	}
	//}
	//

	
	//DualQuaternionBlending(animatedPositions, animatedNormals, dquats, weights, jointsID);

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

//glm::vec3 RotatePoint(glm::quat q, const glm::vec3& v)
//{
//
//	// The conventionnal way to rotate a vector
//	/*
//	Quat_cu tmp = *this;
//	tmp.normalize();
//	// Compute the quaternion inverse with
//	Quat_cu inv = tmp.conjugate();
//	// Compute q * v * inv; in order to rotate the vector v
//	// to do so v must be expressed as the quaternion q(0, v.x, v.y, v.z)
//	return (Vec3)(*this * Quat_cu(0, v) * inv);
//	*/
//
//	// An optimized way to compute rotation
//	glm::vec3 q_vec = glm::vec3(q.x, q.y, q.z);
//	return v + glm::cross((q_vec * 2.f), (glm::cross(q_vec, (v))) + v * q.w);
//}
//
//glm::vec3 TransformPoint(glm::dualquat dq, glm::vec3 p)
//{
//	// As the dual quaternions may be the results from a
//// linear blending we have to normalize it :
//	float norm = glm::length(dq.real);
//	glm::quat qblend_0 = dq.real / norm;
//	glm::quat qblend_e = dq.dual / norm;
//
//	// Translation from the normalized dual quaternion equals :
//	// 2.f * qblend_e * conjugate(qblend_0)
//	glm::vec3 v0 = glm::vec3(qblend_0.x, qblend_0.y, qblend_0.z);
//	glm::vec3 ve = glm::vec3(qblend_e.x, qblend_e.y, qblend_e.z);
//	glm::vec3 trans = (ve * qblend_0.w - v0 * qblend_e.w + cross(v0, ve)) * 2.f;
//
//	// Rotate
//	return RotatePoint(qblend_0, p) + trans;
//}
//
//void Mesh::DualQuaternionBlending(std::vector<glm::vec3>& out_verts, std::vector<glm::vec3>& out_normals,
//	const std::vector<glm::dualquat>& dual_quat, const std::vector<std::vector<float>>& weights,
//	const std::vector<std::vector<int>>& joints_id)
//{
//	for (unsigned v = 0; v < positions.size(); ++v)
//	{
//		const int nb_joints = weights[v].size(); // Number of joints influencing vertex p
//
//		// Init dual quaternion with first joint transformation
//		int   k0 = -1;
//		float w0 = 0.f;
//		glm::dualquat dq_blend;
//		glm::quat q0;
//
//		if (nb_joints != 0)
//		{
//			k0 = joints_id[v][0];
//			w0 = weights[v][0];
//		}
//		else
//			dq_blend = glm::dualquat(glm::quat(1,0,0,0), glm::vec3(0,0,0));
//
//		if (k0 != -1) dq_blend = dual_quat[k0] * w0;
//
//		int pivot = k0;
//
//		q0 = dual_quat[pivot].real;
//		// Look up the other joints influencing 'p' if any
//		for (int j = 1; j < nb_joints; j++)
//		{
//			const int k = joints_id[v][j];
//			float w = weights[v][j];
//			const glm::dualquat& dq = (k == -1) ? glm::dualquat(glm::quat(1, 0, 0, 0), glm::vec3(0, 0, 0)) : dual_quat[k];
//
//			if (glm::dot(dq.real, q0) < 0.f)
//				w *= -1.f;
//
//			dq_blend = dq_blend + dq * w;
//		}
//
//		// Compute animated position
//		glm::vec3 vi = TransformPoint(dq_blend, positions[v]);
//		out_verts[v] = vi;
//		
//		// Compute animated normal
//		out_normals[v] = RotatePoint(glm::normalize(dq_blend.real), normals[v]);
//	}
//}

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
