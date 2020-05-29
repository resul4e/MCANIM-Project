#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

/**
 * \brief A checkpoint that stores the transformation for a single bone at a certain time.
 * These are then interpolated with the previous or latter KeyFrame to get the value at any time.
 */
class KeyFrame
{
public:
	/**
	 * \brief 
	 * \param _time The time of this KeyFrame in frametime.
	 * \param _position The position of the joint at this KeyFrame.
	 * \param _rotation The rotation of the joint at this KeyFrame.
	 * \param _scale The scale of the joint at this KeyFrame.
	 */
	KeyFrame(float _time, glm::vec3 _position, glm::dquat _rotation, glm::vec3 _scale);
	~KeyFrame() = default;
	
	/**
	 * \brief Get the local transform of this keyframe.	
	 * \return The local transform.
	 */
	glm::mat4x4 GetLocalTransform() const;

	/**
	 * \brief Returns the time of this keyframe.
	 * \return The time of this keyframe
	 */
	float GetTime();
	
private:

	/**
	 * \brief Converts the quaternion into a 4x4 rotation matrix.
	 * \param _rotation The rotation to convert.
	 * \return The converted 4x4 matrix.
	 */
	static glm::mat4x4 RotToMat4(glm::quat _rotation);
	
	float m_time;
	glm::vec3 m_position;
	glm::dquat m_rotation;
	glm::vec3 m_scale;
};
