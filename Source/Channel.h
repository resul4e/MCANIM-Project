#pragma once
#include "glm/gtc/quaternion.hpp"
#include "glm/glm.hpp"
#include <vector>
#include <memory>
#include <string>

class KeyFrame;

/**
 * \brief Stores all KeyFrames for a single bone. When asked for a value it will interpolate between the two nearest KeyFrames.
 */
class Channel
{
public:
	/**
	 * \brief Create a new channel that will house all keyframes for this animation for a single joint
	 * \param _jointName The name of the joint that this channel is associated with.
	 */
	Channel(std::string _jointName);
	~Channel() = default;

	/**
	 * \brief Gets the name of the joint this Channel is affecting.
	 * \return The name of the joint this Channel is associated with.
	 */
	std::string GetJointName() const;

	/**
	 * \brief Adds a keyframe to the list of keyframes.
	 * \note There is nothing forcing the keyframes to be ordered by time.
	 * \param _keyFrame The keyframe to add.
	 */
	void AddKeyFrame(std::unique_ptr<KeyFrame> _keyFrame);

	/**
	 * \brief Returns a value that is interpolated between the closest two keyframes of the passed in time. 
	 * \param _time Which keyframe we want.
	 * \return The local transform for this time.
	 */
	glm::mat4x4 GetValue(float _time);

	/**
	 * \brief Interpolates between two KeyFrames and returns the local transform.
	 * \param _prev The index of the previous KeyFrame.
	 * \param _current The index of the current KeyFrame.
	 * \param _a The value between 0 and 1 to interpolate between.
	 * \return The local transform matrix of the interpolation between the two KeyFrames.
	 */
	glm::mat4x4 GetLocalTransform(size_t _prev, size_t _current, double _a) const;

private:

	/**
	* \brief Converts the quaternion into a 4x4 rotation matrix.
	* \param _rotation The rotation to convert.
	* \return The converted 4x4 matrix.
	*/
	static glm::mat4x4 RotToMat4(glm::quat _rotation);
	
	std::vector<std::unique_ptr<KeyFrame>> m_keyFrames;
	std::string m_jointName;
};
