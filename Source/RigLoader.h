#pragma once
#include <memory>
#include <filesystem>

class Rig;
class Joint;
struct aiNode;


/**
 * \brief Class Responsible of loading a Rig and filling it with all of the correct Joints.
 */
class RigLoader
{
public:
	RigLoader() = delete;
	~RigLoader() = delete;

	/**
	 * \brief Loads a rig from file and returns this rig.
	 * \param _filePath The file to load the rig from
	 * \return The rig filled with all the joints in the _filePath
	 */
	static std::shared_ptr<Rig> LoadRig(std::filesystem::path _filePath);

private:
	
	/**
	 * \brief Recursively get all children and create Joints for them
	 * \return the root joint with all of its children
	 */
	static std::shared_ptr<Joint> ExtractChild(aiNode* _node);
};