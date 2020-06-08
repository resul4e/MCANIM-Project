#include "Rig.h"
#include "Joint.h"

Rig::Rig() :
m_root(nullptr)
{
}

void Rig::SetRootJoint(std::shared_ptr<Joint> _joint)
{
	m_root = _joint;
}

std::shared_ptr<Joint> Rig::GetRootJoint() const
{
	return m_root;
}

std::vector<std::shared_ptr<Joint>> Rig::GetAllJoints() const
{
	std::vector<std::shared_ptr<Joint>> joints;

	GetAllJoints(joints, m_root);
	
	return joints;
}

void Rig::GetAllJoints(std::vector<std::shared_ptr<Joint>>& _joints, std::shared_ptr<Joint> _parent) const
{
	for(auto child : _parent->GetChildren())
	{
		_joints.push_back(child);
		GetAllJoints(_joints, child);
	}
}

std::shared_ptr<Joint> Rig::GetJoint(std::string _jointName) const
{
	std::vector<std::shared_ptr<Joint>> allJoints = GetAllJoints();
	for(auto joint : allJoints)
	{
		if(joint->GetName() == _jointName)
		{
			return joint;
		}
	}
	return nullptr;
}
