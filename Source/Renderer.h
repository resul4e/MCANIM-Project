#pragma once

#include "Camera.h"
#include "Scene.h"
#include "Shader.h"
#include "Texture.h"

#include <filesystem>
#include <memory>

class Renderer
{
public:
	Renderer();
	void Initialize(std::filesystem::path _assetPath);
	void Update(Scene& scene);
	void SetBackgroundTexture(std::shared_ptr<Texture> texture);
	void ToggleRigRendering();

private:
	void RenderSky(Scene& scene);
	void RenderModel(Scene& scene);
	void RenderRig(Scene& scene);

	bool renderRig;
	
	Camera camera;
	Shader* shader;
	Shader* rigShader;
	Shader* skyShader;
	std::shared_ptr<Texture> skyTexture;
	std::shared_ptr<Model> m_bone;

	unsigned int m_dummyVao;
	unsigned int m_armatureVao;
	unsigned int m_armaturePbo;
};
