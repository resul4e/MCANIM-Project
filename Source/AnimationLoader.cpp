#include "AnimationLoader.h"
#include "AnimationClip.h"
#include "Channel.h"

#include <cassert>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>


void AnimationLoader::LoadAnimation(std::filesystem::path _filePath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(_filePath.string(), aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
	
	assert(scene->mNumAnimations > 0);
	
	std::shared_ptr<AnimationClip> animation = std::make_shared<AnimationClip>(scene->mAnimations[0]->mName.C_Str(), scene->mAnimations[0]->mTicksPerSecond);

	for(size_t c = 0; c < scene->mAnimations[0]->mNumChannels; c++)
	{
		auto aiChannel = scene->mAnimations[0]->mChannels[c];
		animation->AddChannelMap(aiChannel->mNodeName.C_Str());

		assert(aiChannel->mNumPositionKeys == aiChannel->mNumRotationKeys && aiChannel->mNumRotationKeys == aiChannel->mNumScalingKeys);
		
		for(size_t k = 0; k < aiChannel->mNumPositionKeys; k++)
		{
			glm::vec3 pos(aiChannel->mPositionKeys[k].mValue.x, aiChannel->mPositionKeys[k].mValue.y, aiChannel->mPositionKeys[k].mValue.z);
			glm::dquat rot(aiChannel->mRotationKeys[k].mValue.x, aiChannel->mRotationKeys[k].mValue.y, aiChannel->mRotationKeys[k].mValue.z, aiChannel->mRotationKeys[k].mValue.w);
			glm::vec3 scale(aiChannel->mScalingKeys[k].mValue.x, aiChannel->mScalingKeys[k].mValue.y, aiChannel->mScalingKeys[k].mValue.z);
			
			
			std::unique_ptr<Channel> channel = std::make_unique<Channel>(aiChannel->mPositionKeys[k].mTime, pos, rot, scale);
			animation->AddChannel(aiChannel->mNodeName.C_Str(), std::move(channel));
		}
	}
}
