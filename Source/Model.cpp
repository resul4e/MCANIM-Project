#include "Model.h"

#include "Rig.h"
#include "Joint.h"

#include <glad/glad.h>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

void Mesh::UpdateVertices(const Rig& rig)
{
	glBindVertexArray(vao);

	std::vector<glm::vec3> animatedPositions(positions.size(), glm::vec3(0));
	std::vector<glm::vec3> animatedNormals(normals.size(), glm::vec3(0));
	for (const Bone& bone : m_bones)
	{
		std::shared_ptr<Joint> joint = rig.GetJoint(bone.m_name);
		if (joint == nullptr) continue;

		const glm::mat4& offsetMatrix = bone.m_offsetMatrix;
		glm::mat4 globalTransform = joint->GetGlobalTransform();
		glm::mat4 finalTransform = globalTransform * offsetMatrix;

		for (const VertexWeight& vertexWeight : bone.m_weights)
		{
			const int& vIndex = vertexWeight.m_vertexIndex;
			const float& vWeight = vertexWeight.m_weight;
			glm::vec4 v = glm::vec4(positions[vIndex], 1);
			animatedPositions[vIndex] += glm::vec3(finalTransform * v) * vWeight;
			animatedNormals[vIndex] += glm::mat3(finalTransform) * normals[vertexWeight.m_vertexIndex] * vWeight;
		}
	}

	std::vector<glm::vec3> linearPositions(faces.size() * 3);
	std::vector<glm::vec3> linearNormals(faces.size() * 3);
	std::vector<glm::vec2> linearTextureCoords(faces.size() * 3);

	for (int i = 0; i < faces.size(); i++)
	{
		Face& face = faces[i];
		for (int v = 0; v < 3; v++)
		{
			linearPositions[i * 3 + v] = animatedPositions[face.indices[v]];
			linearNormals[i * 3 + v] = animatedNormals[face.indices[v]];
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
