#include "ArcBall.h"

#include "Scene.h"

#include <glm/glm.hpp>

void ArcBall::Engage()
{
	tracing = true;
}

void ArcBall::Move(Scene& scene, int width, int height, float x, float y)
{
	if (!tracing) return;

	prevX = mx;
	prevY = my;
	if (prevX < 0) { prevX = x; prevY = y; }
	mx = x;
	my = y;

	Camera& camera = scene.GetCamera();
	float dx = x - prevX;
	float dy = y - prevY;
	camera.rotation.x -= dy * 0.01f;
	camera.rotation.y -= dx * 0.01f;
	if (camera.rotation.x > glm::radians(80.0f)) camera.rotation.x = glm::radians(80.0f);
	if (camera.rotation.x < glm::radians(-80.0f)) camera.rotation.x = glm::radians(-80.0f);

	glm::mat4 rotMat(1);
	rotMat = glm::rotate(rotMat, camera.rotation.y, glm::vec3(0, 1, 0));
	rotMat = glm::rotate(rotMat, camera.rotation.x, glm::vec3(1, 0, 0));

	glm::vec3 modelCenter = scene.GetModel().m_bounds.getCenter();
	glm::vec4 camPos(0, 0, camera.distance, 1);
	glm::vec3 rotPos = modelCenter + glm::vec3(rotMat * camPos);
	camera.position = rotPos;
}

void ArcBall::Release()
{
	tracing = false;
	prevX = -1;
	prevY = -1;
	mx = -1;
	my = -1;
}
