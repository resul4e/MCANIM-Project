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

private:
	void RenderModel(Scene& scene);
	void RenderRig(Scene& scene);

	Camera camera;
	Shader* shader;
	Texture* texture;
	std::shared_ptr<Model> m_bone;
};
