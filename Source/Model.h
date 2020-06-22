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

	/**
	 * \brief Upload the mesh data to the graphics card video memory
	 * \remark Needs a valid OpenGL context to be bound when this function is called
	 */
	void Upload();

	/**
	 * \brief Update vertex positions and normals from the underlying animated rig for this particular mesh
	 * \param _rig The rig that is influencing the model geometry
	 * \param _skinningMethod The blending method for combining multiple bones' transformation matrices
	 */
	void UpdateVertices(const Rig& _rig, SkinningMethod _skinningMethod);

	/**
	 * \brief Toggle the skinning method from Linear Blend Skinning to Dual Quaternion and vice versa
	 */
	void ToggleSkinning();

	/** Array of vertex positions belonging to this mesh */
	std::vector<glm::vec3> positions;
	/** Array of vertex texture coordinates belonging to this mesh */
	std::vector<glm::vec2> texCoords;
	/** Array of vertex normals belonging to this mesh */
	std::vector<glm::vec3> normals;
	/** Array of faces which index into the vertex arrays and form geometry */
	std::vector<Face> faces;
	/** Array of bones containing an offset matrix and vertex weights */
	std::vector<Bone> m_bones;

	/** Mediatory array that stores skinned vertex positions used for uploading to the GPU */
	std::vector<glm::vec3> animatedPositions;
	/** Mediatory array that stores skinned vertex normals used for uploading to the GPU */
	std::vector<glm::vec3> animatedNormals;

	/** The skinner that will transform the vertices according to the chosen blending algorithm */
	std::shared_ptr<Skinning> skinner;

	// Graphics vertex buffer objects
	unsigned int vao;
	unsigned int pbo;
	unsigned int nbo;
	unsigned int tbo;

	bool m_dqb = true;
};

class Model
{
public:
	/**
	 * \brief Upload the model data to the graphics card video memory
	 * \remark Needs a valid OpenGL context to be bound when this function is called
	 */
	void Upload();

	/**
	 * \brief Update vertex positions and normals from the underlying animated rig for the whole model
	 * \param _rig The rig that is influencing the model geometry
	 * \param _skinningMethod The blending method for combining multiple bones' transformation matrices
	 */
	void UpdateVertices(const Rig& _rig, SkinningMethod _skinningMethod);

	std::vector<Mesh> m_meshes;
	Bounds m_bounds;
};
