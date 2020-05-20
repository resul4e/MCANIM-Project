#pragma once
#include "glm/gtc/quaternion.hpp"
#include "glm/glm.hpp"
#include <vector>
#include <memory>
#include <string>

class KeyFrame;

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

private:
	std::vector<std::unique_ptr<KeyFrame>> m_keyFrames;
	std::string m_jointName;
};
