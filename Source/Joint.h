#pragma once
#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>

class Joint : public std::enable_shared_from_this<Joint>
{
public:
	Joint(std::string _name, glm::mat4x4 _localTransform);
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

	glm::mat4x4 m_localTransform;
};
