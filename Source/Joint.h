#pragma once
#include <memory>
#include <vector>

class Joint : public std::enable_shared_from_this<Joint>
{
public:
	Joint(std::shared_ptr<Joint> _parent);
	~Joint() = default;

	void AddJoint();
	
private:
	std::shared_ptr<Joint> m_parent;
	std::vector<std::shared_ptr<Joint>> m_children;
};
