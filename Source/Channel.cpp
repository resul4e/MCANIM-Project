#include "Channel.h"
#include "KeyFrame.h"

Channel::Channel(std::string _jointName) :
m_jointName(_jointName)
{
}

std::string Channel::GetJointName() const
{
	return m_jointName;
}

void Channel::AddKeyFrame(std::unique_ptr<KeyFrame> _keyFrame)
{
	m_keyFrames.push_back(std::move(_keyFrame));
}

glm::mat4x4 Channel::GetValue(float _time)
{
	return m_keyFrames[(int)_time]->GetLocalTransform();
}
