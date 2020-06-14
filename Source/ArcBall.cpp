#include "ArcBall.h"

#include "Scene.h"

#include <glm/glm.hpp>
#include <algorithm>

glm::vec3 getArcballVector(float x, float y)
{
	glm::vec3 P = glm::vec3(x / 800 * 2 - 1, y / 800 * 2 - 1, 0);
	P.y = -P.y;
	float opSqrt = P.x * P.x + P.y * P.y;
	if (opSqrt < 1)
		P.z = sqrt(1 - opSqrt);
	else
		P = glm::normalize(P);

	return P;
}

void ArcBall::Engage()
{
	tracing = true;
}

void ArcBall::Move(Scene& scene, float x, float y)
{
	if (!tracing) return;

	prevX = mx;
	prevY = my;
	if (prevX < 0) { prevX = x; prevY = y; }
	mx = x;
	my = y;

	glm::vec3 A = getArcballVector(prevX, prevY);
	glm::vec3 B = getArcballVector(mx, my);
	glm::quat q = glm::quat(-glm::dot(A, B), glm::cross(A, B));
	scene.rot = q * glm::quat(1, 0, 0, 0);
	scene.GetCamera().position = glm::mat4_cast(scene.rot) * glm::vec4(scene.GetCamera().position, 1);
}

void ArcBall::Release()
{
	tracing = false;
	prevX = -1;
	prevY = -1;
	mx = -1;
	my = -1;
}
