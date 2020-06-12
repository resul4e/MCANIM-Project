#include "DQS.h"
#include "Joint.h"
#include "Rig.h"
#include "Model.h"
#include "glmConversion.h"

glm::vec3 RotatePoint(glm::quat q, const glm::vec3& v)
{
	glm::vec3 q_vec = glm::vec3(q.x, q.y, q.z);
	return v + glm::cross((q_vec * 2.f), (glm::cross(q_vec, (v))) + v * q.w);
}

glm::vec3 TransformPoint(glm::dualquat dq, glm::vec3 p)
{
	// As the dual quaternions may be the results from a
	// linear blending we have to normalize it :
	float norm = glm::length(dq.real);
	glm::quat qblend_0 = dq.real / norm;
	glm::quat qblend_e = dq.dual / norm;

	// Translation from the normalized dual quaternion equals :
	// 2.f * qblend_e * conjugate(qblend_0)
	glm::vec3 v0 = glm::vec3(qblend_0.x, qblend_0.y, qblend_0.z);
	glm::vec3 ve = glm::vec3(qblend_e.x, qblend_e.y, qblend_e.z);
	glm::vec3 trans = (ve * qblend_0.w - v0 * qblend_e.w + cross(v0, ve)) * 2.f;

	// Rotate
	return RotatePoint(qblend_0, p) + trans;
}

void DQS::Skin(const Rig& _rig, Mesh& _mesh)
{
	std::vector<glm::dualquat> dquats;

	std::vector<std::vector<float>> weights(_mesh.positions.size(), std::vector<float>());
	std::vector<std::vector<int>> jointsID(_mesh.positions.size(), std::vector<int>());

	for (int i = 0; i < _mesh.m_bones.size(); i++)
	{
		const Bone& bone = _mesh.m_bones[i];
		std::shared_ptr<Joint> joint = _rig.GetJoint(bone.m_name);
		if (joint == nullptr) continue;

		const glm::mat4& offsetMatrix = bone.m_offsetMatrix;
		glm::mat4 globalTransform = joint->GetGlobalTransform();
		glm::mat4 finalTransform = globalTransform * offsetMatrix;

		auto trans = Mat4ToQuat(finalTransform);
		auto dq = CreateDualQuat(trans, glm::vec3(finalTransform[3][0], finalTransform[3][1], finalTransform[3][2]));

		dquats.push_back(dq);

		for (const VertexWeight& vertexWeight : bone.m_weights)
		{
			const int& vIndex = vertexWeight.m_vertexIndex;
			const float& vWeight = vertexWeight.m_weight;

			weights[vIndex].push_back(vWeight);
			jointsID[vIndex].push_back(i);
		}
	}

	Skin(_mesh.positions, _mesh.normals, _mesh.animatedPositions, _mesh.animatedNormals, dquats, weights, jointsID);
}

void DQS::Skin(const std::vector<glm::vec3>& i_verts, const std::vector<glm::vec3>& i_normals,
               std::vector<glm::vec3>& o_verts, std::vector<glm::vec3>& o_normals, const std::vector<glm::dualquat>& _dualQuat,
               const std::vector<std::vector<float>>& _weights, const std::vector<std::vector<int>>& _jointIDs)
{
	for (unsigned v = 0; v < i_verts.size(); ++v)
	{
		const int nb_joints = _weights[v].size(); // Number of joints influencing vertex p

		// Init dual quaternion with first joint transformation
		int   k0 = -1;
		float w0 = 0.f;
		glm::dualquat dq_blend;
		glm::quat q0;

		if (nb_joints != 0)
		{
			k0 = _jointIDs[v][0];
			w0 = _weights[v][0];
		}
		else
			dq_blend = glm::dualquat(glm::quat(1, 0, 0, 0), glm::vec3(0, 0, 0));

		if (k0 != -1) dq_blend = _dualQuat[k0] * w0;

		int pivot = k0;

		q0 = _dualQuat[pivot].real;
		// Look up the other joints influencing 'p' if any
		for (int j = 1; j < nb_joints; j++)
		{
			const int k = _jointIDs[v][j];
			float w = _weights[v][j];
			const glm::dualquat& dq = (k == -1) ? glm::dualquat(glm::quat(1, 0, 0, 0), glm::vec3(0, 0, 0)) : _dualQuat[k];

			if (glm::dot(dq.real, q0) < 0.f)
				w *= -1.f;

			dq_blend = dq_blend + dq * w;
		}

		// Compute animated position
		glm::vec3 vi = TransformPoint(dq_blend, i_verts[v]);
		o_verts[v] = vi;

		// Compute animated normal
		o_normals[v] = RotatePoint(glm::normalize(dq_blend.real), i_normals[v]);
	}
}
