#include "ArcBall.h"

#include "Scene.h"

#include <glm/glm.hpp>
#include <algorithm>

glm::vec3 FindConstrainedPoint(glm::vec3 fp, glm::vec3 axis)
{
	float d = glm::dot(fp, axis);
	glm::vec3 proj = fp - (axis * d);
	float norm = glm::length(proj);
	glm::vec3 P = fp;
	if (norm > 0)
	{
		float s = 1 / norm;
		if (proj.z < 0)
			s = -s;
		P = proj * s;
	}
	else if (axis.z == 1.0)
	{
		P = glm::vec3(1, 0, 0);
	}
	else
		P = glm::normalize(glm::vec3(-axis.y, axis.x, 0));
	return P;
}

glm::vec3 GetArcballVector(int width, int height, float x, float y)
{
	glm::vec3 P = glm::vec3(x / (width*2) * 2 - 1, y / (height*2) * 2 - 1, 0);
	P.y = -P.y;
	float opSqrt = P.x * P.x + P.y * P.y;
	if (opSqrt < 1)
		P.z = sqrt(1 - opSqrt);
	else
		P = glm::normalize(P);

	glm::vec3 Yaxis(0, 1, 0);
	glm::vec3 Xaxis(1, 0, 0);

	glm::vec3 xp = FindConstrainedPoint(P, Xaxis);
	glm::vec3 yp = FindConstrainedPoint(P, Yaxis);

	return yp;
}

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

	glm::vec3 A = GetArcballVector(width, height, prevX, prevY);
	glm::vec3 B = GetArcballVector(width, height, mx, my);
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
