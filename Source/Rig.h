#pragma once
#include <memory>

class Joint;

class Rig
{
public:
	Rig();
	~Rig() = default;

	void SetRootJoint(std::shared_ptr<Joint> _joint);
	std::shared_ptr<Joint> GetRootJoint() const;
	
private:
	std::shared_ptr<Joint> m_root;
};
