#pragma once

#include "Bounds.h"

#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <glm/gtx/dual_quaternion.hpp>
#include <memory>

class Skinning;
enum class SkinningMethod;

struct Face
{
	unsigned int indices[3];
};

/**
 * \brief The weight for a single vertex. Stores both the vertex index for the mesh and the weight.
 * \remark All of the weights for a single vertex should sum up to 1.
 */
struct VertexWeight
{
	unsigned int m_vertexIndex;
	float m_weight;
};

/**
 * \brief A "Bone" that stores the weights for each vertex.
 * \remark This is a mirror of how the data is layed out in Assimp, we could possibly improve this by storing this
 * data in the rig instead. However, this would mean that each rig can only be used on a single model.
 */
struct Bone
{
	std::string m_name;
	std::vector<VertexWeight> m_weights;
	glm::mat4 m_offsetMatrix;
};

class Rig;

struct Mesh
{
	Mesh();

	void Upload();
	void UpdateVertices(const Rig& rig, SkinningMethod skinningMethod);
	void ToggleSkinning();

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<Face> faces;
	std::vector<Bone> m_bones;

	std::vector<glm::vec3> animatedPositions;  
	std::vector<glm::vec3> animatedNormals;

	std::shared_ptr<Skinning> skinner;

	unsigned int vao;
	unsigned int pbo;
	unsigned int nbo;
	unsigned int tbo;

	bool m_dqb = true;
};

class Model
{
public:
	void Upload();
	void UpdateVertices(const Rig& rig, SkinningMethod skinningMethod);

	std::vector<Mesh> meshes;
	Bounds m_bounds;
};
