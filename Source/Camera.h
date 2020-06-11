#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera(float _fov, float _aspect, float _zNear, float _zFar);

	void SetAspectRatio(float aspect);
	void loadProjectionMatrix(glm::mat4& projMatrix);

	glm::vec3 position;
	float distance;

private:
	float m_fov;
	float m_aspect;
	float m_zNear;
	float m_zFar;
};
