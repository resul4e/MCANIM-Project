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
	const int index = static_cast<int>(std::ceil(_time));
	const int prevIndex = index == 0 ? index : index - 1;
	
	if(m_keyFrames.size() <= index || m_keyFrames.size() < 2)
	{
		return GetLocalTransform(m_keyFrames.size() - 1, m_keyFrames.size() - 1, 0);
	}
	
	return GetLocalTransform(prevIndex, index, static_cast<double>(_time) - prevIndex);
}

glm::mat4x4 Channel::GetLocalTransform(size_t _prev, size_t _current, double _a) const
{
	glm::vec3 lerpPos = mix(m_keyFrames[_prev]->GetPosition(), m_keyFrames[_current]->GetPosition(), _a);
	glm::dquat lerpRot = slerp(m_keyFrames[_prev]->GetRotation(), m_keyFrames[_current]->GetRotation(), _a);
	glm::vec3 lerpScale = mix(m_keyFrames[_prev]->GetScale(), m_keyFrames[_current]->GetScale(), _a);

	glm::mat4x4 rotTransform = RotToMat4(lerpRot);

	glm::mat4x4 localTransform{ 1 };
	localTransform = glm::translate(localTransform, lerpPos);
	localTransform *= rotTransform;
	localTransform = glm::scale(localTransform, lerpScale);

	return localTransform;
}

glm::mat4x4 Channel::RotToMat4(glm::quat _rotation)
{
	float xx = _rotation.x * _rotation.x;
	float xy = _rotation.x * _rotation.y;
	float xz = _rotation.x * _rotation.z;
	float xw = _rotation.x * _rotation.w;
	float yy = _rotation.y * _rotation.y;
	float yz = _rotation.y * _rotation.z;
	float yw = _rotation.y * _rotation.w;
	float zz = _rotation.z * _rotation.z;
	float zw = _rotation.z * _rotation.w;

	glm::mat4x4 rotMatrix{ 1 };
	rotMatrix[0][0] = 1 - 2 * (yy + zz);
	rotMatrix[1][0] = 2 * (xy - zw);
	rotMatrix[2][0] = 2 * (xz + yw);
	rotMatrix[0][1] = 2 * (xy + zw);
	rotMatrix[1][1] = 1 - 2 * (xx + zz);
	rotMatrix[2][1] = 2 * (yz - xw);
	rotMatrix[0][2] = 2 * (xz - yw);
	rotMatrix[1][2] = 2 * (yz + xw);
	rotMatrix[2][2] = 1 - 2 * (xx + yy);

	return rotMatrix;
}
