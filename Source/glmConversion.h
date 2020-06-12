#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/dual_quaternion.hpp>

glm::quat Mat4ToQuat(glm::mat4 _m)
{
	// Compute trace of matrix 't'
	float T = 1 + _m[0][0] + _m[1][1] + _m[2][2];

	float S, X, Y, Z, W;

	if (T > 0.00000001f) // to avoid large distortions!
	{
		S = sqrt(T) * 2.f;
		X = (_m[2][1] - _m[1][2]) / S;
		Y = (_m[0][2] - _m[2][0]) / S;
		Z = (_m[1][0] - _m[0][1]) / S;
		W = 0.25f * S;
	}
	else
	{
		if (_m[0][0] > _m[1][1] && _m[0][0] > _m[1][1])
		{
			// Column 0 :
			S = sqrt(1.0f + _m[0][0] - _m[1][1] - _m[2][2]) * 2.f;
			X = 0.25f * S;
			Y = (_m[1][0] + _m[0][1]) / S;
			Z = (_m[0][2] + _m[2][0]) / S;
			W = (_m[2][1] - _m[1][2]) / S;
		}
		else if (_m[1][1] > _m[2][2])
		{
			// Column 1 :
			S = sqrt(1.0f + _m[1][1] - _m[0][0] - _m[2][2]) * 2.f;
			X = (_m[1][0] + _m[0][1]) / S;
			Y = 0.25f * S;
			Z = (_m[2][1] + _m[1][2]) / S;
			W = (_m[0][2] - _m[2][0]) / S;
		}
		else
		{   // Column 2 :
			S = sqrt(1.0f + _m[2][2] - _m[0][0] - _m[1][1]) * 2.f;
			X = (_m[0][2] + _m[2][0]) / S;
			Y = (_m[2][1] + _m[1][2]) / S;
			Z = 0.25f * S;
			W = (_m[1][0] - _m[0][1]) / S;
		}
	}

	return glm::quat(W, -X, -Y, -Z);
}

glm::dualquat CreateDualQuat(const glm::quat& q, const glm::vec3& t)
{
	float w = -0.5f * (t.x * q.x + t.y * q.y + t.z * q.z);
	float i = 0.5f * (t.x * q.w + t.y * q.z - t.z * q.y);
	float j = 0.5f * (-t.x * q.z + t.y * q.w + t.z * q.x);
	float k = 0.5f * (t.x * q.y - t.y * q.x + t.z * q.w);

	return glm::dualquat(q, glm::quat(w, i, j, k));
}
