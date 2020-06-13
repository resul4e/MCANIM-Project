#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

class DualQuat
{
public:
	DualQuat()
	{
		DualQuat res = dual_quat_from(glm::quat(), glm::vec3(0.f, 0.f, 0.f));
		*this = res;
	}
	
	DualQuat(glm::quat _q0, glm::quat _qe)
	{
		q0 = _q0;
		qe = _qe;
	}

	DualQuat(const glm::mat4& t)
	{
		glm::quat q(t);
		glm::vec3 translation(t[0][3], t[1][3], t[2][3]);
		DualQuat res = dual_quat_from(q, translation);
		*this = res;
	}
	
	static DualQuat dual_quat_from(const glm::quat& q, const glm::vec3& t)
	{
		float w = -0.5f * (t.x * q.x + t.y * q.y + t.z * q.z);
		float i = 0.5f * (t.x * q.w + t.y * q.z - t.z * q.y);
		float j = 0.5f * (-t.x * q.z + t.y * q.w + t.z * q.x);
		float k = 0.5f * (t.x * q.y - t.y * q.x + t.z * q.w);

		return DualQuat(q, glm::quat(w, i, j, k));
	}

	static DualQuat identity()
	{
		return DualQuat(glm::quat(1.f, 0.f, 0.f, 0.f),
			glm::vec3(0.f, 0.f, 0.f));
	}

	DualQuat operator+(const DualQuat& dq) const
	{
		return DualQuat(q0 + dq.q0, qe + dq.qe);
	}

	DualQuat operator*(float scalar) const
	{
		return DualQuat(q0 * scalar, qe * scalar);
	}

	glm::vec3 transform(const glm::vec3& p) const
	{
		// As the dual quaternions may be the results from a
		// linear blending we have to normalize it :
		float norm = glm::length(q0);
		glm::quat qblend_0 = q0 / norm;
		glm::quat  qblend_e = qe / norm;

		// Translation from the normalized dual quaternion equals :
		// 2.f * qblend_e * conjugate(qblend_0)
		glm::vec3 v0 = glm::vec3(qblend_0.x, qblend_0.y, qblend_0.z);
		glm::vec3 ve = glm::vec3(qblend_e.x, qblend_e.y, qblend_e.z);
		glm::vec3 trans = (ve * qblend_0.w - v0 * qblend_e.w+ glm::cross(v0, ve)) * 2.f;

		// Rotate
		return rotate(qblend_0, p) + trans;
	}
	
	glm::quat rotation() const { return q0; }

private:

	glm::quat q0;
	glm::quat qe;
};
