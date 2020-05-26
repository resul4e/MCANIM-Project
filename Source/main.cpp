#include "Window.h"
#include "Renderer.h"
#include "RigLoader.h"
#include "Rig.h"
#include "AnimationLoader.h"
#include "ModelLoader.h"
#include "Model.h"

#include <iostream>

int main(int argc, char** argv)
{
	std::vector<std::string> cmdArgs(argv, argv + argc);

	if (cmdArgs.size() < 2)
	{
		std::cout << "Executable syntax: <SkeletalAnimator.exe> <asset_path>" << std::endl;
		return 0;
	}
	std::filesystem::path assetPath = cmdArgs[1];

	Window window;
	Renderer renderer;

	//loading the test rig.
	std::shared_ptr<Rig> rig = RigLoader::LoadRig(assetPath.string() + "/Idle.fbx");
	std::shared_ptr<AnimationClip> anim = AnimationLoader::LoadAnimation(assetPath.string() + "/Idle.fbx");
	std::shared_ptr<Model> model = ModelLoader::LoadModel(assetPath.string() + "/Idle.fbx");

	window.create("Skeletal Animator", 800, 800);

	model->Upload();
	renderer.Initialize(assetPath);
	renderer.SetModel(model);
	renderer.SetRig(rig);

	while (window.isOpen())
	{
		renderer.Update();

		window.render();
		window.update();
	}

	return 0;
}
