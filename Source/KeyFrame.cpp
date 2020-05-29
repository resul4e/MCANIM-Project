#include "KeyFrame.h"
#include "glm/gtx/quaternion.hpp"

KeyFrame::KeyFrame(float _time, glm::vec3 _position, glm::dquat _rotation, glm::vec3 _scale) :
	m_time(_time),
	m_position(_position),
	m_rotation(_rotation),
	m_scale(_scale)
{
	
}

glm::mat4x4 KeyFrame::GetLocalTransform() const
{
	glm::mat4x4 localTransform{1};
	glm::mat4x4 posMatrix{ 1 };
	glm::mat4x4 scaleMatrix{ 1 };

	scaleMatrix[0][0] = m_scale.x;
	scaleMatrix[1][1] = m_scale.y;
	scaleMatrix[2][2] = m_scale.z;

	posMatrix[0][3] = m_position.x;
	posMatrix[1][3] = m_position.y;
	posMatrix[2][3] = m_position.z;

	glm::mat4x4 rotTransform = RotToMat4(m_rotation);

	localTransform *= scaleMatrix;
	localTransform *= rotTransform;
	localTransform *= posMatrix;
	
	return localTransform;
}

float KeyFrame::GetTime()
{
	return m_time;
}

glm::mat4x4 KeyFrame::RotToMat4(glm::quat _rotation)
{
	float xx = _rotation.x * _rotation.x;
	float xy = _rotation.x * _rotation.y;
	float xz = _rotation.x * _rotation.z;
	float xw = _rotation.x * _rotation.w;
	float yy = _rotation.y * _rotation.y;
	float yz = _rotation.y * _rotation.z;
	float yw = _rotation.y * _rotation.w;
	float zz = _rotation.z * _rotation.z;
	float zw = _rotation.z * _rotation.w;

	glm::mat4x4 rotMatrix{ 1 };
	rotMatrix[0][0] = 1 - 2 * (yy + zz);
	rotMatrix[0][1] = 2 * (xy - zw);
	rotMatrix[0][2] = 2 * (xz + yw);
	rotMatrix[1][0] = 2 * (xy + zw);
	rotMatrix[1][1] = 1 - 2 * (xx + zz);
	rotMatrix[1][2] = 2 * (yz - xw);
	rotMatrix[2][0] = 2 * (xz - yw);
	rotMatrix[2][1] = 2 * (yz + xw);
	rotMatrix[2][2] = 1 - 2 * (xx + yy);

	return rotMatrix;
}
