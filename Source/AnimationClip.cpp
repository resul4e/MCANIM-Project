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