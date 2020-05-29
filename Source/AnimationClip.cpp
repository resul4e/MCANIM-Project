#include "AnimationClip.h"
#include "Channel.h"
#include "KeyFrame.h"

AnimationClip::AnimationClip(std::string _animationName, float _fps, float _duration) :
	m_animationName(_animationName),
	m_fps(_fps),
	m_duration(_duration)
{
}

void AnimationClip::AddChannel(std::shared_ptr<Channel> _channel)
{
	m_channels.insert(m_channels.end(), { _channel->GetJointName(), _channel });
}

bool AnimationClip::HasChannel(std::string _name) const
{
	return m_channels.find(_name) != m_channels.end();
}

std::shared_ptr<Channel> AnimationClip::GetChannel(std::string _name) const
{
	assert(HasChannel(_name));
	
	return m_channels.at(_name);
}

float AnimationClip::GetDuration() const
{
	return m_duration;
}

float AnimationClip::GetFPS() const
{
	assert(abs(m_fps) > std::numeric_limits<float>::epsilon());
	
	return m_fps;
}

std::string AnimationClip::GetName() const
{
	return m_animationName;
}
