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
	//Get the first keframe that is in the future
	int index = 0;
	for(int i = 0; i < m_keyFrames.size(); i++)
	{
		if(m_keyFrames[i]->GetTime() > _time)
		{
			index = i;
			break;
		}
	}

	//If we are out of bounds somehow, or we don't have enough keyframes to interpolate, just use the last frame.
	if (m_keyFrames.size() <= index || m_keyFrames.size() < 2)
	{
		return GetLocalTransform(m_keyFrames.size() - 1, m_keyFrames.size() - 1, 0);
	}

	//Get the prev index to interpolate between
	const int prevIndex = index == 0 ? index : index - 1;

	//See how much time is in between the two keyframes
	float diffTime = m_keyFrames[index]->GetTime() - m_keyFrames[prevIndex]->GetTime();
	if(prevIndex == index)
	{
		diffTime = 1;
	}

	//Interpolate between the two keyframes and return the result.
	return GetLocalTransform(prevIndex, index, (_time - m_keyFrames[prevIndex]->GetTime()) / diffTime);
}

glm::mat4x4 Channel::GetLocalTransform(size_t _prev, size_t _current, double _a) const
{
	//lerp the position and scale, use slerp for rotation.
	glm::vec3 lerpPos = mix(m_keyFrames[_prev]->GetPosition(), m_keyFrames[_current]->GetPosition(), _a);
	glm::dquat lerpRot = slerp(m_keyFrames[_prev]->GetRotation(), m_keyFrames[_current]->GetRotation(), _a);
	glm::vec3 lerpScale = mix(m_keyFrames[_prev]->GetScale(), m_keyFrames[_current]->GetScale(), _a);

	//Get the rotation in matrix form.
	glm::mat4x4 rotTransform = RotToMat4(lerpRot);

	//Apply position, rotation and lastly scaling and return it.
	glm::mat4x4 localTransform{ 1 };
	localTransform = glm::translate(localTransform, lerpPos);
	localTransform *= rotTransform;
	localTransform = glm::scale(localTransform, lerpScale);

	return localTransform;
}

glm::mat4x4 Channel::RotToMat4(glm::quat _rotation)
{
	/*
	 * There is a glm function that can convert a quaternion into a 4x4 matrix,
	 * but this does not seem to output the correct matrix. So we do it by hand.
	 */
	
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
