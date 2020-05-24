#pragma once

#include <glm/glm.hpp>

#include <vector>

struct Face
{
	unsigned int i0, i1, i2;
};

struct Mesh
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<Face> faces;
};

class Model
{
public:
	std::vector<Mesh> meshes;
};
