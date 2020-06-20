#pragma once
#include "Skinning.h"

#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtx/dual_quaternion.hpp>


/**
 * \brief Class that is resposible of attaching the Mesh to the Rig using the Dual Quaternion method first described in
 * https://www.cs.utah.edu/~ladislav/kavan07skinning/kavan07skinning.pdf.
 * \remark DQS code inspired by http://rodolphe-vaillant.fr/?e=29.
 */
class DualQuaternionSkinning : public Skinning
{
public:
	DualQuaternionSkinning() = default;
	~DualQuaternionSkinning() = default;

	/**
	 * \brief Uses dual quaternions to represent rotation and translation and then uses this to skin the skeleton.
	 * \param _rig The rig we are going to wrap the Mesh around
	 * \param _mesh The Mesh we are going to wrap around the Rig.
	 */
	void Skin(const Rig& _rig, Mesh& _mesh) override;

private:
	
	static void Skin(const std::vector<glm::vec3>& i_verts, const std::vector<glm::vec3>& i_normals,
	          std::vector<glm::vec3>& o_verts, std::vector<glm::vec3>& o_normals,
	          const std::vector<glm::dualquat>& _dualQuat, const std::vector<std::vector<float>>& _weights,
	          const std::vector<std::vector<int>>& _jointIDs);
};
