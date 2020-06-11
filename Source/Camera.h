#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera(float _fov, float _aspect, float _zNear, float _zFar);

	void SetAspectRatio(float aspect);
	void loadProjectionMatrix(glm::mat4& projMatrix);
	void LookAt(glm::mat4& m, const glm::vec3& eye, const glm::vec3& center, const glm::vec3& top);

	glm::vec3 position;
	float distance;

private:
	float m_fov;
	float m_aspect;
	float m_zNear;
	float m_zFar;
};
