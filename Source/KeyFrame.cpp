#include "KeyFrame.h"
#include "glm/gtx/quaternion.hpp"

KeyFrame::KeyFrame(float _time, glm::vec3 _position, glm::dquat _rotation, glm::vec3 _scale) :
	m_time(_time),
	m_position(_position),
	m_rotation(_rotation),
	m_scale(_scale)
{
	
}

glm::mat4x4 KeyFrame::GetLocalTransform()
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

	glm::mat4x4 rotTransform = glm::toMat4(m_rotation);

	localTransform *= scaleMatrix;
	localTransform *= rotTransform;
	localTransform *= posMatrix;
	
	return localTransform;
}
