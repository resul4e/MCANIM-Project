#pragma once

#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include "Texture.h"

#include <filesystem>

class Model;

class Renderer
{
public:
	Renderer();
	void Initialize(std::filesystem::path _assetPath);
	void Update();
	void SetupQuad();
	void SetModel(std::shared_ptr<Model> model);
private:
	Camera camera;
	Shader* shader;
	Texture* texture;

	std::shared_ptr<Model> m_model;
};
