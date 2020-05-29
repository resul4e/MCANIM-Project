#include <iostream>

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
	std::shared_ptr<AnimationClip> anim = AnimationLoader::LoadAnimation(assetPath.string() + "/Idle.fbx");

	AnimationPlayer player(scene, ModelLoader::LoadModel(assetPath.string() + "/Idle.fbx"), RigLoader::LoadRig(assetPath.string() + "/Idle.fbx"));
	
	window.create("Skeletal Animator", 800, 800);

	scene->GetModel().Upload();
	renderer.Initialize(assetPath);

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window.GetWindow(), true);
	ImGui::StyleColorsDark();

	double time;
	while (window.isOpen())
	{
		renderer.Update(*scene);
		
		ImGui_ImplGlfwGL3_NewFrame();
		ImGui::ShowDemoWindow();
		
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		window.render();
		window.update();
	}

	ImGui_ImplGlfwGL3_Shutdown();

	return 0;
}
