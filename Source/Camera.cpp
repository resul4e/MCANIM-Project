#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float _fov, float _aspect, float _zNear, float _zFar) :
	m_fov(_fov),
	m_aspect(_aspect),
	m_zNear(_zNear),
	m_zFar(_zFar)
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
