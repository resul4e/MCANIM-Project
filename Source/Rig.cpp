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
