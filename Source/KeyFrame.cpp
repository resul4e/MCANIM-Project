#include "KeyFrame.h"

KeyFrame::KeyFrame(float _time, glm::vec3 _position, glm::dquat _rotation, glm::vec3 _scale) :
	m_time(_time),
	m_position(_position),
	m_rotation(_rotation),
	m_scale(_scale)
{
	
}