#pragma once

#include <glm/glm.hpp>

class Bounds
{
public:
	Bounds();
	Bounds(glm::vec3 _min, glm::vec3 _max);

	glm::vec3 getMin();
	glm::vec3 getMax();
	glm::vec3 getCenter();
	float getMaxDimension();

private:
	glm::vec3 m_min;
	glm::vec3 m_max;
	glm::vec3 m_center;
};
