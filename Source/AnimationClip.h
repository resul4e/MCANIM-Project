#pragma once
#include <map>
#include <string>
#include <memory>
#include <vector>

class Channel;

/**
 * \brief A single animation. Holds a list of keyframes for each bone.
 */
class AnimationClip
{
public:
	/**
	 * \brief Create a new animation.
	 * \param _animationName The name of the animation
	 * \param _fps How many frames are in a second.
	 */
	AnimationClip(std::string _animationName, float _fps, float _duration);
	~AnimationClip() = default;

	/**
	 * \brief Adds a new channel. Each channel stores a list of all keyframes for a single joint.
	 * \param _channel The new channel to be added.
	 */
	void AddChannel(std::shared_ptr<Channel> _channel);

	/**
	 * \brief Checks if we have a Channel with the given name.
	 * \param _name The name to check.
	 * \return True if there is a Channel with that name, False otherwise.
	 */
	bool HasChannel(std::string _name) const;
	
	/**
	 * \brief Returns the channel that corresponds to the given name.
	 * \param _name The name of the channel we want to get.
	 * \return The channel with the name _name.
	 */
	std::shared_ptr<Channel> GetChannel(std::string _name) const;

	/**
	 * \brief How long the animation takes to complete. Also depends on the FPS
	 * \return The duration of the animation
	 * \see GetFPS()
	 */
	float GetDuration() const;
	
	/**
	 * \brief How many frames should be shown per second.
	 * \return The FPS of the animation.
	 */
	float GetFPS() const;

	/**
	 * \brief Returns the name of the animation (This is mixamo.com for all of then, but we have added the file name to it to separate them)
	 * \return The name of the animation
	 */
	std::string GetName() const;

private:
	std::string m_animationName;
	float m_fps;
	float m_duration;

	std::map<std::string, std::shared_ptr<Channel>> m_channels;
	std::map<std::string, std::vector<std::unique_ptr<Channel>>> m_channelMap;
};
