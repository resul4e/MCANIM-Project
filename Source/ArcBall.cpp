#include "ArcBall.h"

#include "Camera.h"

#include <glm/glm.hpp>

void ArcBall::Engage()
{
	tracing = true;
}

void ArcBall::Move(Camera& _camera, int _width, int _height, float _x, float _y)
{
	// If the arc ball is currently not engaged don't move anything
	if (!tracing) return;

	// Keep track of the previously stored coordinates
	m_prevX = m_x;
	m_prevY = m_y;
	// If the coordinates had not been set yet, because this is the first frame, set them to the given coordinates
	if (m_prevX < 0) { m_prevX = _x; m_prevY = _y; }

	// Store the given coordinates
	m_x = _x;
	m_y = _y;

	// Compute the difference between the stored coordinates
	float dx = _x - m_prevX;
	float dy = _y - m_prevY;

	// Adjust the camera rotation based on the differential movement
	_camera.rotation.x -= dy * 0.01f;
	_camera.rotation.y -= dx * 0.01f;

	// Limit how far the camera is allowed to pivot on the pitch axis
	if (_camera.rotation.x > glm::radians(80.0f)) _camera.rotation.x = glm::radians(80.0f);
	if (_camera.rotation.x < glm::radians(-80.0f)) _camera.rotation.x = glm::radians(-80.0f);

	// Compute the new position of the camera according to its rotation and distance from the center
	_camera.RecomputePosition();
}

void ArcBall::Release()
{
	tracing = false;

	// Reset the stored coordinates
	m_prevX = -1;
	m_prevY = -1;
	m_x = -1;
	m_y = -1;
}
