#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float _fov, float _aspect, float _zNear, float _zFar) :
	m_fov(_fov),
	m_aspect(_aspect),
	m_zNear(_zNear),
	m_zFar(_zFar),
	rotation(0, 0, 0),
	center(0, 0, 0)
{

}

void Camera::SetAspectRatio(float aspect)
{
	m_aspect = aspect;
}

void Camera::loadProjectionMatrix(glm::mat4& projMatrix)
{
	projMatrix = glm::perspective(m_fov, m_aspect, m_zNear, m_zFar);
}

void Camera::LookAt(glm::mat4& m, const glm::vec3& eye, const glm::vec3& center, const glm::vec3& top)
{
	glm::vec3 forward = normalize(center - eye);
	glm::vec3 side = normalize(cross(forward, top));
	glm::vec3 up = cross(side, forward);

	m = glm::mat4(1);
	m[0][0] = side.x;
	m[0][1] = up.x;
	m[0][2] = -forward.x;
	m[1][0] = side.y;
	m[1][1] = up.y;
	m[1][2] = -forward.y;
	m[2][0] = side.z;
	m[2][1] = up.z;
	m[2][2] = -forward.z;

	m = glm::translate(m, -eye);
}

void Camera::RecomputePosition()
{
	glm::mat4 rotMat(1);
	rotMat = glm::rotate(rotMat, rotation.y, glm::vec3(0, 1, 0));
	rotMat = glm::rotate(rotMat, rotation.x, glm::vec3(1, 0, 0));

	glm::vec4 camPos(0, 0, distance, 1);
	glm::vec3 rotPos = center + glm::vec3(rotMat * camPos);
	position = rotPos;
}
