#include <iostream>
#include <chrono>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include "RigLoader.h"
#include "AnimationLoader.h"
#include "ModelLoader.h"

#include "Window.h"
#include "Renderer.h"
#include "Scene.h"
#include "Model.h"

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
	auto scene = std::make_shared<Scene>();
	

	//loading the test rig.
	std::shared_ptr<AnimationClip> anim = AnimationLoader::LoadAnimation(assetPath.string() + "/SkinningTest.fbx");

	AnimationPlayer player(scene, ModelLoader::LoadModel(assetPath.string() + "/SkinningTest.fbx"), RigLoader::LoadRig(assetPath.string() + "/SkinningTest.fbx"));
	player.AddAnimation(anim);

	window.create("Skeletal Animator", 800, 800);

	scene->GetModel().Upload();
	renderer.Initialize(assetPath);

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window.GetWindow(), true);
	ImGui::StyleColorsDark();

	auto time = std::chrono::high_resolution_clock::now();
	double dt = 0.016;
	while (window.isOpen())
	{
		player.Update(dt);
		renderer.Update(*scene);
		
		ImGui_ImplGlfwGL3_NewFrame();
		ImGui::ShowDemoWindow();

		player.ImGuiRender();

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		window.render();
		window.update();

		//get deltatime
		std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - time;
		dt = elapsed.count();
		time = std::chrono::high_resolution_clock::now();
	}

	ImGui_ImplGlfwGL3_Shutdown();

	return 0;
}
