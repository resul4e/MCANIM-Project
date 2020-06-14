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

	camera.RecomputePosition();
}

void ArcBall::Release()
{
	tracing = false;
	prevX = -1;
	prevY = -1;
	mx = -1;
	my = -1;
}
