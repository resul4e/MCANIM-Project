#pragma once
#include <filesystem>

class AnimationClip;

/**
 * \brief The class that can load an animation and set the data up for our framework.
 */
class AnimationLoader
{
public:
	AnimationLoader() = delete;
	~AnimationLoader() = delete;
	
	/**
	 * \brief Loads an AnimationClip, the associated Channels and its KeyFrames and returns the pointer to the AnimationClip.
	 * \param _filePath The file that the animation is stored in.
	 * \return The AnimationClip that stores all data.
	 */
	static std::shared_ptr<AnimationClip> LoadAnimation(std::filesystem::path _filePath);
};