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
#include "FBXLoader.h"
#include "ModelSelector.h"

#include <vector>
#include <string>

class Application : public ResizeListener, public MouseListener
{
public:
	void Initialize(std::filesystem::path assetPath)
	{
		window.Create("Skeletal Animator", 1440, 1080);
		window.SetWindowIcon(assetPath.string() + "/Icon.png");
		window.AddResizeListener(this);
		window.AddMouseListener(this);

		interfaceController.Setup(window);

		// Load all fbx files
		FBXLoader::assetPath = assetPath.string();
		FBXLoader::ScanNew();
		for (std::shared_ptr<AnimationClip> animation : FBXLoader::fbxAnimations){
			player.AddAnimation(animation);
		}
		player.SetModel(scene, FBXLoader::fbxModels.front());

		scene.SetGroundPlane(ModelLoader::LoadModel(assetPath.string() + "/Plane.obj"));
		scene.GetGroundPlane().Upload();

		renderer.Initialize(assetPath);

		std::shared_ptr<Texture> texture = std::make_shared<Texture>(assetPath.string() + "/FuzZombie__diffuse.png");
		scene.SetTexture(texture);

		std::shared_ptr<Texture> specularMap = std::make_shared<Texture>(assetPath.string() + "/FuzZombie__specular.png");
		scene.SetSpecularMap(specularMap);

		std::shared_ptr<Texture> skyTexture = std::make_shared<Texture>(assetPath.string() + "/Background.jpg");
		renderer.SetBackgroundTexture(skyTexture);

		OnResize(window.GetWidth(), window.GetHeight());
	}

	void Update()
	{
		auto time = std::chrono::high_resolution_clock::now();
		double dt = 0.016;
		while (window.IsOpen())
		{

			// Create begin frame method
			interfaceController.Begin();

			player.Update(scene, dt);
			renderer.Update(scene);

			CreateMainMenuBar();
			
			if (ImGui::IsKeyPressed('S' /*S*/))
			{
				for(Mesh& m : scene.GetModel().m_meshes)
				{
					m.ToggleSkinning();
				}
			}

			// ImGui rendering
			player.ImGuiRender(scene);

			interfaceController.End();
			window.Render();
			window.Update();

			//get deltatime
			std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - time;
			dt = elapsed.count();
			time = std::chrono::high_resolution_clock::now();
		}

		interfaceController.ShutDown();
	}

	void OnResize(int width, int height) override
	{
		renderer.Resize(scene, width, height);
	}

	void OnMouseClicked(int button, int mods) override
	{
		scene.GetArcBall().Engage();
	}

	void OnMouseReleased(int button, int mods) override
	{
		scene.GetArcBall().Release();
	}

	void OnMouseMove(float x, float y) override
	{
		if(!ImGui::IsAnyItemHovered() && !ImGui::IsAnyWindowHovered())
		{
			scene.GetArcBall().Move(scene.GetCamera(), window.GetWidth(), window.GetHeight(), x, y);
		}
	}

	void OnMouseScroll(float xOffset, float yOffset) override
	{
		scene.GetCamera().distance -= yOffset * scene.GetCamera().distance * 0.06f;
		scene.GetCamera().RecomputePosition();
	}

private:

	void CreateMainMenuBar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Windows"))
			{
				if (ImGui::MenuItem("Animation Player", "G", player.IsGuiOpen()))
				{
					player.ToggleImguiWindow();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Settings"))
			{
				if (ImGui::MenuItem("Use Dual Quaternion Skinning", "S", player.GetSkinningMethod() == SkinningMethod::DUAL_QUATERNION))
				{
					player.SwitchSkinning();
				}
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}
	
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
