#pragma once

#include "Camera.h"
#include "Scene.h"
#include "Shader.h"
#include "Texture.h"

#include <filesystem>

class Renderer
{
public:
	Renderer();
	void Initialize(std::filesystem::path _assetPath);
	void Update(Scene& scene);
	void SetupQuad();
	void ToggleRigRendering();

private:
	void RenderModel(Scene& scene);
	void RenderRig(Scene& scene);

	bool renderRig;
	
	Camera camera;
	Shader* shader;
	Shader* rigShader;
	Texture* texture;
	std::shared_ptr<Model> m_bone;

	unsigned int m_armatureVao;
	unsigned int m_armaturePbo;
};
