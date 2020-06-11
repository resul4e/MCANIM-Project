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

#include <vector>
#include <string>

std::vector<std::string> animationPaths
{
	"Idle.fbx",
	"SkinningTest.fbx",
	"Capoeira.fbx",
	"Samba Dancing.fbx",
	"Hit Reaction.fbx",
	"Prone Left Turn.fbx",
	"Turn Right.fbx"
};

class Application
{
public:
	void Initialize(std::filesystem::path assetPath)
	{
		scene.SetRig(RigLoader::LoadRig(assetPath.string() + "/Idle.fbx"));
		scene.SetModel(ModelLoader::LoadModel(assetPath.string() + "/Idle.fbx"));

		// Load all animations from file and add them to the player
		for (const std::string& animationPath : animationPaths)
		{
			std::shared_ptr<AnimationClip> anim = AnimationLoader::LoadAnimation(assetPath.string() + "/" + animationPath);
			player.AddAnimation(anim);
		}

		window.create("Skeletal Animator", 800, 800);
		interfaceController.Setup(window);

		scene.GetModel().Upload();
		renderer.Initialize(assetPath);
		//renderer.Resize(window.)

		std::shared_ptr<Texture> texture = std::make_shared<Texture>(assetPath.string() + "/FuzZombie__diffuse.png");
		scene.SetTexture(texture);

		std::shared_ptr<Texture> skyTexture = std::make_shared<Texture>(assetPath.string() + "/Background.jpg");
		renderer.SetBackgroundTexture(skyTexture);
	}

	void Update()
	{
		auto time = std::chrono::high_resolution_clock::now();
		double dt = 0.016;
		while (window.isOpen())
		{

			// Create begin frame method
			interfaceController.Begin();

			player.Update(scene, dt);
			renderer.Update(scene);

			if (ImGui::IsKeyPressed('J' /*J*/))
			{
				renderer.ToggleRigRendering();
			}

			// ImGui rendering
			player.ImGuiRender();

			interfaceController.End();
			window.render();
			window.update();

			//get deltatime
			std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - time;
			dt = elapsed.count();
			time = std::chrono::high_resolution_clock::now();
		}

		interfaceController.ShutDown();
	}

	//void OnResize(int width, int height) override
	//{
	//	
	//}

private:
	Window window;
	Renderer renderer;
	Scene scene;
	InterfaceController interfaceController;
	AnimationPlayer player;
};

int main(int argc, char** argv)
{
	std::vector<std::string> cmdArgs(argv, argv + argc);

	if (cmdArgs.size() < 2)
	{
		std::cout << "Executable syntax: <SkeletalAnimator.exe> <asset_path>" << std::endl;
		return 0;
	}
	std::filesystem::path assetPath = cmdArgs[1];

	Application application;
	application.Initialize(assetPath);
	application.Update();

	return 0;
}
