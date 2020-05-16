#include "Joint.h"

Joint::Joint(std::string _name, glm::mat4x4 _localTransform) :
m_name(_name),
m_parent(nullptr),
m_localTransform(_localTransform)
{
}

void Joint::AddJoint(std::shared_ptr<Joint> _child)
{
	_child->SetParent(shared_from_this());
	m_children.push_back(_child);
}

void Joint::SetParent(std::shared_ptr<Joint> _parent)
{
	m_parent = _parent;
}

glm::mat4x4 Joint::GetLocalTransform() const
{
	return m_localTransform;
}

glm::mat4x4 Joint::GetGlobalTransform() const
{
	if(m_parent == nullptr)
	{
		return glm::mat4(1.0f);
	}
	return m_parent->GetGlobalTransform() * m_localTransform;
}
