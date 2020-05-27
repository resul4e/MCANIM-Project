#pragma once

#include <glm/glm.hpp>

#include <vector>

struct Face
{
	unsigned int indices[3];
};

struct Mesh
{
	void Upload();

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<Face> faces;

	unsigned int vao;
	unsigned int pbo;
	unsigned int nbo;
};

class Model
{
public:
	void Upload();

	std::vector<Mesh> meshes;
	glm::vec3 minBounds;
	glm::vec3 maxBounds;
};
