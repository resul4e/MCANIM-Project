#pragma once
#include <filesystem>
#include <map>

class AnimationClip;
class Rig;
class Model;

/**
 * \brief The class that can load FBX files and stores the loaded components (animation, rig, model)
 */
class FBXLoader {
public:
	static std::filesystem::path assetPath;
	static std::vector<std::filesystem::path> files;
	static std::vector<std::shared_ptr<AnimationClip>> fbxAnimations;
	static std::vector<std::pair<std::string, std::shared_ptr<Rig>>> fbxRigs;
	static std::vector<std::pair<std::string, std::shared_ptr<Model>>> fbxModels;

	static std::vector<std::filesystem::path> ScanNew();
};
