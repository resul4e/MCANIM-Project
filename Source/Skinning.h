#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include <glm/gtx/dual_quaternion.hpp>
#include "Model.h"

class Skinning
{
public:
	Skinning() = default;
	virtual ~Skinning() = default;

	/**
	 * \brief Attaches a Mesh to a Rig using the bone weights saved in the Mesh.
	 * \param _rig The rig to attached the skin to.
	 * \param _mesh The mesh that holds the skin we are going to attach to the bones.
	 */
	virtual void Skin(const Rig& _rig, Mesh& _mesh) = 0;

};
