//DQS code inspired by http://rodolphe-vaillant.fr/?e=29

#pragma once
#include "Skinning.h"

class DQS : public Skinning
{
public:
	DQS() = default;
	~DQS() = default;

	void Skin(const Rig& _rig, Mesh& _mesh) override;

private:
	void Skin(const std::vector<glm::vec3>& i_verts, const std::vector<glm::vec3>& i_normals,
	          std::vector<glm::vec3>& o_verts, std::vector<glm::vec3>& o_normals,
	          const std::vector<glm::dualquat>& _dualQuat, const std::vector<std::vector<float>>& _weights,
	          const std::vector<std::vector<int>>& _jointIDs);
};
