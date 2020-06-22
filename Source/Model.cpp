#include "Model.h"

#include "Rig.h"
#include "Joint.h"
#include "SkinningMethod.h"
#include "LinearBlendSkinning.h"
#include "DualQuaternionSkinning.h"

#include <glad/glad.h>
#include <iostream>

Mesh::Mesh() :
	skinner(std::make_shared<DualQuaternionSkinning>())
{

}

void Mesh::UpdateVertices(const Rig& _rig, SkinningMethod _skinningMethod)
{
	// Create a new skinner according to the chosen blending algorithm
	if (_skinningMethod == SkinningMethod::LINEAR_BLEND)
		skinner = std::make_shared<LinearBlendSkinning>();
	else if (_skinningMethod == SkinningMethod::DUAL_QUATERNION)
		skinner = std::make_shared<DualQuaternionSkinning>();

	// Bind the vertex array object in VRAM
	glBindVertexArray(vao);

	// Reset the transformed vertex positions and normals to 0 so they can be set by the skinner
	animatedPositions = std::vector<glm::vec3>(positions.size(), glm::vec3(0));
	animatedNormals = std::vector<glm::vec3>(normals.size(), glm::vec3(0));
	
	// Perform the skinning of the mesh vertices according to the chosen blending algorithm
	skinner->Skin(_rig, *this);

	// Go through all faces and linearize the vertex data for uploading to the graphics card
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

void Mesh::ToggleSkinning()
{
	m_dqb = !m_dqb;
	if (m_dqb)
	{
		skinner = std::make_shared<DualQuaternionSkinning>();
	}
	else
	{
		skinner = std::make_shared<LinearBlendSkinning>();
	}
}

void Mesh::Upload()
{
	// Go through all faces and linearize the vertex data for uploading to the graphics card
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

	// Generate the initial vertex array and buffer objects
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

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
	for (Mesh& mesh : m_meshes)
	{
		mesh.Upload();
	}
}

void Model::UpdateVertices(const Rig& _rig, SkinningMethod _skinningMethod)
{
	for (Mesh& mesh : m_meshes)
	{
		mesh.UpdateVertices(_rig, _skinningMethod);
	}
}
