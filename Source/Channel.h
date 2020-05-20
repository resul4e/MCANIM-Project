#pragma once
#include "glm/gtc/quaternion.hpp"
#include "glm/glm.hpp"
#include <string>

class Channel
{
public:
	Channel(float _time, glm::vec3 _position, glm::dquat _rotation, glm::vec3 _scale);
	~Channel() = default;

private:
	float m_time;
	glm::vec3 m_position;
	glm::dquat m_rotation;
	glm::vec3 m_scale;
};
