#pragma once

#include "Skinning.h"

class LinearBlendSkinning : public Skinning
{
public:
	LinearBlendSkinning() = default;
	~LinearBlendSkinning() = default;

	/** \see Skinning::Skin()
	 */
	void Skin(const Rig& _rig, Mesh& _mesh) override;
};
