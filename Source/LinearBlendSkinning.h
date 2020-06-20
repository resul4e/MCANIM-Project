#pragma once
#include "Skinning.h"

/**
 * \brief Class that is resposible of attaching the Mesh to the Rig using the Linear interpolation.
 */
class LinearBlendSkinning : public Skinning
{
public:
	LinearBlendSkinning() = default;
	~LinearBlendSkinning() = default;

	/**
	 *\copydoc Skinning::Skin()
	 *\see Skinning::Skin()
	 */
	void Skin(const Rig& _rig, Mesh& _mesh) override;
};
