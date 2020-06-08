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
	for (Bone& bone : m_bones)
	{
		std::shared_ptr<Joint> joint = rig.GetJoint(bone.m_name);

		if (joint != nullptr)
		{
			for (VertexWeight& vertexWeight : bone.m_weights)
			{
				glm::mat4 globalTransform = joint->GetGlobalTransform();
				glm::mat4 offsetMatrix = bone.m_offsetMatrix;
				
				glm::vec4 v = glm::vec4(positions[vertexWeight.m_vertexIndex], 1);
				animatedPositions[vertexWeight.m_vertexIndex] += glm::vec3(globalTransform * offsetMatrix * v) * vertexWeight.m_weight;
			}
		}
	}

	std::vector<glm::vec3> linearPositions(faces.size() * 3);
	std::vector<glm::vec3> linearNormals(faces.size() * 3);
	
	for (int i = 0; i < faces.size(); i++)
	{
		Face& face = faces[i];
		for (int v = 0; v < 3; v++)
		{
			linearPositions[i * 3 + v] = animatedPositions[face.indices[v]];
			linearNormals[i * 3 + v] = normals[face.indices[v]];
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
	std::cout << faces.size() << std::endl;
	for (int i = 0; i < faces.size(); i++)
	{
		Face& face = faces[i];
		for (int v = 0; v < 3; v++)
		{
			linearPositions[i * 3 + v] = positions[face.indices[v]];
			linearNormals[i * 3 + v] = normals[face.indices[v]];
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
