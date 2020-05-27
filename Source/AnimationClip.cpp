#include "AnimationClip.h"
#include "Channel.h"
#include "KeyFrame.h"

AnimationClip::AnimationClip(std::string _animationName, float _fps) :
	m_animationName(_animationName),
	m_fps(_fps)
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
