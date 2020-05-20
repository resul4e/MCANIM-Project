#pragma once
#include <map>
#include <string>
#include <memory>
#include <vector>

class Channel;

class AnimationClip
{
public:
	AnimationClip(std::string _animationName, float _fps);
	~AnimationClip() = default;

	void AddChannel(std::shared_ptr<Channel> _channel);

private:
	std::string m_animationName;
	float m_fps;

	std::map<std::string, std::shared_ptr<Channel>> m_channels;
	std::map<std::string, std::vector<std::unique_ptr<Channel>>> m_channelMap;
};
