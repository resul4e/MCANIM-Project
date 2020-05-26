#pragma once

#include "Camera.h"
#include "Model.h"
#include "Rig.h"
#include "Shader.h"
#include "Texture.h"

#include <filesystem>

class Renderer
{
public:
	Renderer();
	void Initialize(std::filesystem::path _assetPath);
	void Update();
	void SetupQuad();
	void SetModel(std::shared_ptr<Model> model);
	void SetRig(std::shared_ptr<Rig> rig);
private:
	void RenderModel();
	void RenderRig();

	Camera camera;
	Shader* shader;
	Texture* texture;
	std::shared_ptr<Model> m_bone;

	std::shared_ptr<Model> m_model;
	std::shared_ptr<Rig> m_rig;
};
