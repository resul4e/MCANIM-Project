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

	void AddChannelMap(std::string _channelName);
	void AddChannel(std::string _channelName, std::unique_ptr<Channel> _channel);

private:
	std::string m_animationName;

	float m_fps;
	
	std::map<std::string, std::vector<std::unique_ptr<Channel>>> m_channelMap;
};
