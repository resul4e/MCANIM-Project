#include "KeyFrame.h"
#include "glm/gtx/quaternion.hpp"

KeyFrame::KeyFrame(float _time, glm::vec3 _position, glm::dquat _rotation, glm::vec3 _scale) :
	m_time(_time),
	m_position(_position),
	m_rotation(_rotation),
	m_scale(_scale)
{
	
}

glm::vec3 KeyFrame::GetPosition() const
{
	return m_position;
}

glm::dquat KeyFrame::GetRotation() const
{
	return m_rotation;
}

glm::vec3 KeyFrame::GetScale() const
{
	return m_scale;
}

float KeyFrame::GetTime() const
{
	return m_time;
}