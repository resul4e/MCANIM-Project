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
namespace RotationalType
{
	static const unsigned int HINGE = 1;
	static const unsigned int UNIVERSAL = 2;
	static const unsigned int BALLSOCKET = 3;
};

namespace TranslationalType
{
	static const unsigned int PRISMATIC = 1;
	static const unsigned int COORDS = 3;
};

/**
 * \brief A single Joint in a Parent-Child structure. Each Joint has one parent and can have multiple children.
 * \see RigLoader::LoadRig().
 */
class Joint : public std::enable_shared_from_this<Joint>
{
public:
	Joint(std::string _name, glm::mat4x4 _localTransform, const JointType _jointType, const int _nDofs, glm::vec3 _offset = glm::vec3(0, 0, 0));
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
	 * \brief Get a list of all children directly contained by this Joint.
	 * \return A list of all of the children.
	 */
	std::vector<std::shared_ptr<Joint>> GetChildren() const;

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
	 * \brief Sets a new Transform for this Joint.
	 * \param _newTransform The new transform for this Joint.
	 */
	void SetLocalTransform(glm::mat4x4 _newTransform);

	/**
	 * \brief Gets the global transform.
	 * \return returns the global transform.
	 */
	glm::mat4x4 GetGlobalTransform() const;

	/**
	 * \brief Returns the name of this Joint. This name will probably match one in a Channel.
	 */
	std::string GetName() const;
	
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