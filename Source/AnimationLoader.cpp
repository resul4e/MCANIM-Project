#include "AnimationLoader.h"
#include "AnimationClip.h"
#include "Channel.h"
#include "KeyFrame.h"
#include <windows.h>

#include <cassert>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>


std::map<std::string, std::shared_ptr<AnimationClip>> AnimationLoader::loadedAnimationClips;

std::shared_ptr<AnimationClip> AnimationLoader::LoadAnimation(std::filesystem::path _filePath)
{
	//Open the file in assimp.
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(_filePath.string(), aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

	//Check that we actually have an animation.
	assert(scene->mNumAnimations > 0);

	//Create the AnimationClip that we will be storing things in.
	std::string animName = scene->mAnimations[0]->mName.C_Str();
	animName += "." + _filePath.filename().replace_extension("").string();
	std::shared_ptr<AnimationClip> animation = std::make_shared<AnimationClip>(animName, scene->mAnimations[0]->mTicksPerSecond, scene->mAnimations[0]->mDuration);

	//Go through all Channels 
	for(size_t c = 0; c < scene->mAnimations[0]->mNumChannels; c++)
	{
		auto aiChannel = scene->mAnimations[0]->mChannels[c];

		//Create a new channel in the AnimationClip
		const std::shared_ptr<Channel> channel = std::make_shared<Channel>(aiChannel->mNodeName.C_Str());
		animation->AddChannel(channel);

		//Check that we have the same number of position, rotation, and scaling keys. I am not sure that this is always the case so if this asserts, we might need to
		//change th logic on this.
		assert(aiChannel->mNumPositionKeys == aiChannel->mNumRotationKeys && aiChannel->mNumRotationKeys == aiChannel->mNumScalingKeys);

		//Go through all of the keyFrames stored in this channel.
		for(size_t k = 0; k < aiChannel->mNumPositionKeys; k++)
		{
			//Extract the position, rotation and scaling vectors.
			glm::vec3 pos(aiChannel->mPositionKeys[k].mValue.x, aiChannel->mPositionKeys[k].mValue.y, aiChannel->mPositionKeys[k].mValue.z);
			glm::dquat rot(aiChannel->mRotationKeys[k].mValue.w, aiChannel->mRotationKeys[k].mValue.x, aiChannel->mRotationKeys[k].mValue.y, aiChannel->mRotationKeys[k].mValue.z);
			glm::vec3 scale(aiChannel->mScalingKeys[k].mValue.x, aiChannel->mScalingKeys[k].mValue.y, aiChannel->mScalingKeys[k].mValue.z);
			
			//Create a new keyFrame and add it to the current channel;
			std::unique_ptr<KeyFrame> keyFrame = std::make_unique<KeyFrame>(static_cast<float>(aiChannel->mPositionKeys[k].mTime), pos, rot, scale);
			channel->AddKeyFrame(std::move(keyFrame));
		}
	}

	return animation;
}

std::vector<std::shared_ptr<AnimationClip>> AnimationLoader::ScanNew() {
	std::vector<std::string> fileNames;
	std::vector<std::shared_ptr<AnimationClip>> animations;

	HANDLE dir;
	WIN32_FIND_DATA file_data;

	if ((dir = FindFirstFile((AnimationLoader::assetPath + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
		return animations; /* No files found */

	do {
		const std::string file_name = file_data.cFileName;
		const std::string full_file_name = AnimationLoader::assetPath + "/" + file_name;
		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (file_name[0] == '.')
			continue;

		if (is_directory)
			continue;

		if (file_name.substr(file_name.find_last_of(".") + 1) == "fbx") {
			fileNames.push_back(full_file_name);
		}
	} while (FindNextFile(dir, &file_data));

	FindClose(dir);


	for (std::string animationPath : fileNames) {
		// Only add if not found in loaded Animation Clips
		if (AnimationLoader::loadedAnimationClips.find(animationPath) == AnimationLoader::loadedAnimationClips.end()) {
			printf("Adding fbx: %s\n", animationPath.c_str());
			std::shared_ptr<AnimationClip> anim = AnimationLoader::LoadAnimation(animationPath);
			AnimationLoader::loadedAnimationClips.insert(std::make_pair(animationPath, anim));
			animations.push_back(anim);
		}
	}

	return animations;
}
