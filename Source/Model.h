#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <string>

struct Face
{
	unsigned int indices[3];
};

struct VertexWeight
{
	unsigned int m_vertexIndex;
	float m_weight;
};

struct Bone
{
	std::string m_name;
	std::vector<VertexWeight> m_weights;
	glm::mat4 m_offsetMatrix;
};

class Rig;

struct Mesh
{
	void Upload();
	void UpdateVertices(const Rig& rig);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<Face> faces;
	std::vector<Bone> m_bones;

	unsigned int vao;
	unsigned int pbo;
	unsigned int nbo;
};

class Model
{
public:
	void Upload();
	void UpdateVertices(const Rig& rig);

	std::vector<Mesh> meshes;
	glm::vec3 minBounds;
	glm::vec3 maxBounds;
};
