#pragma once
#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <array>

enum class JointType
{
	ROTATIONAL,
	TRANSLATIONAL
};
enum class RotationalType
{
	HINGE = 1,
	UNIVERSAL = 2,
	BALLSOCKET = 3
};
enum class TranslationalType
{
	PRISMATIC = 1,
	COORDS = 3
};

class Joint : public std::enable_shared_from_this<Joint>
{
public:
	Joint(std::string _name, glm::mat4x4 _localTransform, const JointType _jointType, const int _nDofs, glm::vec3 _offset);
	~Joint() = default;

	/**
	 * \brief Adds the node to the list of children and makes the child's parent be this.
	 * \param _child The new node to add to the list of children
	 */
	void AddJoint(std::shared_ptr<Joint> _child);
	
	/**
	 * \brief Sets the parent of the Joint to _parent
	 * \param _parent The new parent of this Joint.
	 */
	void SetParent(std::shared_ptr<Joint> _parent);

	/**
	 * \brief Sets the offset of the joint, denoting the pivot point relative to the parent's matrix
	 * \param _offset
	 */
	void SetOffset(glm::vec3 _offset);

	/**
	 * \brief Sets the limits of a single DOF
	 * \param _dof the index of the DOF to set the limits for
	 * \param _min the minimum rotation
	 * \param _max the maximum rotation
	 */
	void SetDOFLimits(int _dof, float _min, float _max);

	/**
	 * \brief Gets the array of DOFs
	 * \return The array of size nDofs of DOFs
	 */
	template<int nDofs>
	std::array<float, nDofs> GetDOFs();

	/**
	 * \brief Returns the local transform.
	 * \return The local transform of this bone relative to its parent.
	 */
	glm::mat4x4 GetLocalTransform() const;

	/**
	 * \brief Gets the global transform.
	 * \return returns the global transform.
	 */
	glm::mat4x4 GetGlobalTransform() const;
	
private:
	std::string m_name;
	std::shared_ptr<Joint> m_parent;
	std::vector<std::shared_ptr<Joint>> m_children;

	const JointType m_jointType;
	const int m_nDofs;

	std::array<float, 3> m_dofs, m_dofMinLimit, m_dofMaxLimit;

	glm::vec3 m_offset;

	glm::mat4x4 m_localTransform;
};