#include "AnimationClip.h"
#include "Channel.h"

AnimationClip::AnimationClip(std::string _animationName, float _fps) :
	m_animationName(_animationName),
	m_fps(_fps)
{
}

void AnimationClip::AddChannelMap(std::string _channelName)
{

	std::vector<std::unique_ptr<Channel>> channels;
	m_channelMap.insert(m_channelMap.begin(), { _channelName, std::move(channels) });
}

void AnimationClip::AddChannel(std::string _channelName, std::unique_ptr<Channel> _channel)
{
	m_channelMap[_channelName].push_back(std::move(_channel));
}
