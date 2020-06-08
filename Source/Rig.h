#pragma once
#include <memory>
#include <vector>
#include <string>

class Joint;


/**
 * \brief Holds the root Joint of one Rig. All Joints together represent a single character's skeleton.
 */
class Rig
{
public:
	Rig();
	~Rig() = default;

	/**
	 * \brief Sets the root Joint of this Rig.
	 * \param _joint The Joint that will be the root of this joint.
	 */
	void SetRootJoint(std::shared_ptr<Joint> _joint);

	/**
	 * \return The Joint that is the Root of this Rig.
	 */
	std::shared_ptr<Joint> GetRootJoint() const;

	/**
	 * \brief Recursively goes through all Joints in the Root Joint and returns it as a list.
	 * \return A list of all of the Joints.
	 */
	std::vector<std::shared_ptr<Joint>> GetAllJoints() const;

	std::shared_ptr<Joint> GetJoint(std::string _jointName) const;
	
private:

	void GetAllJoints(std::vector<std::shared_ptr<Joint>>& _joints, std::shared_ptr<Joint> _parent) const;
	
	std::shared_ptr<Joint> m_root;
};
