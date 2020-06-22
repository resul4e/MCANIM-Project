#pragma once

class Camera;

class ArcBall
{
public:
	/**
	 * \brief Engage interaction with the arc ball
	 */
	void Engage();

	/**
	 * \brief Moves the camera according to the difference between the given x and y
	 * and the previously stored ones
	 * \param _camera The camera for which to compute the new position
	 * \param _width The width the arc ball should occupy on the screen
	 * \param _height The height the arc ball should occupy on the screen
	 * \param _x The x position of the input controller
	 * \param _y The y position of the input controller
	 */
	void Move(Camera& _camera, int _width, int _height, float _x, float _y);

	/**
	 * \brief Release interaction with the arc ball
	 */
	void Release();

private:
	float m_prevX = -1;
	float m_prevY = -1;
	float m_x = -1;
	float m_y = -1;

	bool tracing = false;
};
