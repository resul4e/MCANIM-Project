#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

class KeyFrame
{
public:
	KeyFrame(float _time, glm::vec3 _position, glm::dquat _rotation, glm::vec3 _scale);
	~KeyFrame() = default;

private:
	float m_time;
	glm::vec3 m_position;
	glm::dquat m_rotation;
	glm::vec3 m_scale;
};
