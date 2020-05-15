#include "Joint.h"

Joint::Joint(std::shared_ptr<Joint> _parent) :
m_parent(_parent)
{
}

void Joint::AddJoint()
{
	m_children.push_back(std::make_shared<Joint>(shared_from_this()));
}
