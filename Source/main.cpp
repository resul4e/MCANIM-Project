#include "Window.h"
#include "Renderer.h"
#include "Scene.h"
#include "RigLoader.h"
#include "Rig.h"
#include "AnimationLoader.h"
#include "ModelLoader.h"
#include "Model.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
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
	Scene scene;

	//loading the test rig.
	scene.SetRig(RigLoader::LoadRig(assetPath.string() + "/Idle.fbx"));
	scene.SetModel(ModelLoader::LoadModel(assetPath.string() + "/Idle.fbx"));
	std::shared_ptr<AnimationClip> anim = AnimationLoader::LoadAnimation(assetPath.string() + "/Idle.fbx");

	window.create("Skeletal Animator", 800, 800);

	scene.GetModel().Upload();
	renderer.Initialize(assetPath);

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window.GetWindow(), true);
	ImGui::StyleColorsDark();

	while (window.isOpen())
	{
		renderer.Update(scene);
		
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
