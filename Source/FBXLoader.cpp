#include "FBXLoader.h"

#include <Windows.h>
#include "AnimationLoader.h"
#include "RigLoader.h"
#include "ModelLoader.h"
#include <iostream>

std::filesystem::path FBXLoader::assetPath;
std::vector<std::filesystem::path> FBXLoader::files;
std::vector<std::shared_ptr<AnimationClip>> FBXLoader::fbxAnimations;
std::vector<std::pair<std::string, std::shared_ptr<Rig>>> FBXLoader::fbxRigs;
std::vector<std::pair<std::string, std::shared_ptr<Model>>> FBXLoader::fbxModels;

std::vector<std::filesystem::path> FBXLoader::ScanNew() {
	std::vector<std::filesystem::path> fbxFiles;

	std::filesystem::path path = FBXLoader::assetPath;
	for (const auto& entry : std::filesystem::directory_iterator(path)){
		if (entry.path().extension() == ".fbx") {
			fbxFiles.push_back(entry.path());
		}
	}

	// TODO: check if we are not loading the a pre-existing animation, rig, or model by comparing the data 
	// current checks are placeholders since pointer will be different for every load?
	for (std::filesystem::path fbxPath : fbxFiles) {
		if (std::find(FBXLoader::files.begin(), FBXLoader::files.end(), fbxPath) == FBXLoader::files.end()) {
			std::cout << "Adding fbx: " << fbxPath.filename().u8string() << std::endl;
			files.push_back(fbxPath);

			if (AnimationLoader::HasAnimation(fbxPath)) {
				std::shared_ptr<AnimationClip> anim = AnimationLoader::LoadAnimation(fbxPath);
				FBXLoader::fbxAnimations.push_back(anim);
			}else{
				std::cout << "Couln't find animation in " << fbxPath.filename().u8string() << std::endl;
			}
			
			std::string fileName = fbxPath.filename().string();
			std::string modelPrefix = "Model";
			if (fileName.rfind(modelPrefix, 0) == 0) {
				std::shared_ptr<Rig> rig = RigLoader::LoadRig(fbxPath);
				FBXLoader::fbxRigs.push_back(std::make_pair(fbxPath.filename().u8string(), rig));

				std::shared_ptr<Model> model = ModelLoader::LoadModel(fbxPath);
				FBXLoader::fbxModels.push_back(std::make_pair(fbxPath.filename().u8string(), model));
			}
		}
	}
	return fbxFiles;
}