#include <iostream>
#include <chrono>

#include "imgui.h"

#include "RigLoader.h"
#include "AnimationLoader.h"
#include "ModelLoader.h"

#include "Window.h"
#include "Renderer.h"
#include "Scene.h"
#include "Model.h"
#include "InterfaceController.h"
#include "AnimationPlayer.h"

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
	InterfaceController interfaceController;

	auto scene = std::make_shared<Scene>();

	std::shared_ptr<AnimationClip> anim = AnimationLoader::LoadAnimation(assetPath.string() + "/SkinningTest.fbx");
	std::shared_ptr<AnimationClip> IdleAnim = AnimationLoader::LoadAnimation(assetPath.string() + "/Idle.fbx");
	std::shared_ptr<AnimationClip> capAnim = AnimationLoader::LoadAnimation(assetPath.string() + "/Capoeira.fbx");
	std::shared_ptr<AnimationClip> sambaAnim = AnimationLoader::LoadAnimation(assetPath.string() + "/Samba Dancing.fbx");

	scene->SetRig(RigLoader::LoadRig(assetPath.string() + "/SkinningTest.fbx"));
	scene->SetModel(ModelLoader::LoadModel(assetPath.string() + "/SkinningTest.fbx"));

	AnimationPlayer player(scene);
	player.AddAnimation(IdleAnim);
	player.AddAnimation(anim);
	player.AddAnimation(capAnim);
	player.AddAnimation(sambaAnim);

	window.create("Skeletal Animator", 800, 800);
	interfaceController.Setup(window);

	scene->GetModel().Upload();
	renderer.Initialize(assetPath);

	auto time = std::chrono::high_resolution_clock::now();
	double dt = 0.016;
	while (window.isOpen())
	{
		// Create begin frame method
		interfaceController.Begin();

		player.Update(dt);
		renderer.Update(*scene);
		
		// ImGui rendering
		player.ImGuiRender();
		ImGui::ShowDemoWindow();

		interfaceController.End();
		window.render();
		window.update();

		//get deltatime
		std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - time;
		dt = elapsed.count();
		time = std::chrono::high_resolution_clock::now();
	}

	interfaceController.ShutDown();

	return 0;
}
