#include "Joint.h"

Joint::Joint(std::string _name, glm::mat4x4 _localTransform, const JointType _jointType, const int _nDofs, glm::vec3 _offset) :
m_name(_name),
m_parent(nullptr),
m_localTransform(_localTransform),
m_jointType(_jointType),
m_nDofs(_nDofs),
m_offset(_offset)
{
	m_dofs = m_dofMinLimit = m_dofMaxLimit = {0,0,0};
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

std::vector<std::shared_ptr<Joint>> Joint::GetChildren() const
{
	return m_children;
}

void Joint::SetOffset(glm::vec3 _offset)
{
	m_offset = _offset;
}

void Joint::SetDOFLimits(int _dof, float _min, float _max)
{
	assert(_dof < m_nDofs);
	m_dofMinLimit[_dof] = _min;
	m_dofMaxLimit[_dof] = _max;
}

template<int nDofs>
std::array<float, nDofs> Joint::GetDOFs()
{
	assert(nDofs == m_dofs);
	return m_dofs;
}

glm::mat4x4 Joint::GetLocalTransform() const
{
	return m_localTransform;
}

void Joint::SetLocalTransform(glm::mat4x4 _newTransform)
{
	m_localTransform = _newTransform;
}

glm::mat4x4 Joint::GetGlobalTransform() const
{
	if(m_parent == nullptr)
	{
		return glm::mat4(1.0f);
	}
	return  m_parent->GetGlobalTransform() * m_localTransform;
}

std::string Joint::GetName() const
{
	return m_name;
}
