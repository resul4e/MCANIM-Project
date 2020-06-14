#pragma once

class Scene;

class ArcBall
{
public:
	void Engage();
	void Move(Scene& scene, float x, float y);
	void Release();

private:
	float prevX = -1;
	float prevY = -1;
	float mx = -1;
	float my = -1;

	bool tracing = false;
};
