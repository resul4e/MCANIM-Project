#pragma once
#include <filesystem>
#include <map>

class AnimationClip;

/**
 * \brief The class that can load an animation and set the data up for our framework.
 */
class AnimationLoader
{
public:
	AnimationLoader() = delete;
	~AnimationLoader() = delete;
	
	inline static std::string assetPath;
	static std::map<std::string, std::shared_ptr<AnimationClip>> loadedAnimationClips;

	/**
	 * \brief Loads an AnimationClip, the associated Channels and its KeyFrames and returns the pointer to the AnimationClip.
	 * \param _filePath The file that the animation is stored in.
	 * \return The AnimationClip that stores all data.
	 */
	static std::shared_ptr<AnimationClip> LoadAnimation(std::filesystem::path _filePath);

	static std::vector<std::shared_ptr<AnimationClip>> ScanNew();

};
