#pragma once

class Rig;
class Mesh;

/**
 * \brief Abstract base class for skinning.
 * \remark In your inherited class override Skin() and use it to attach the Mesh to the Rig in a certain way.
 * \see DualQuaternionSkinning
 * \see LinearBlendSkinning
 */
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
