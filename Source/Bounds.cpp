#include "Bounds.h"

#include <algorithm>

Bounds::Bounds() :
	m_min(0, 0, 0),
	m_max(0, 0, 0),
	m_center(0, 0, 0)
{

}

Bounds::Bounds(glm::vec3 _min, glm::vec3 _max) :
	m_min(_min),
	m_max(_max)
{
	m_center = (m_min + m_max) * 0.5f;
}

glm::vec3 Bounds::getMin()
{
	return m_min;
}

glm::vec3 Bounds::getMax()
{
	return m_max;
}

glm::vec3 Bounds::getCenter()
{
	return m_center;
}

float Bounds::getMaxDimension()
{
	glm::vec3 range = (m_max - m_min);
	return std::max(std::max(range.x, range.y), range.z);
}
