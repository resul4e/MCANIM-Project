#pragma once

#include "Model.h"
#include "Shader.h"
#include "Texture.h"

class Model;

class Renderer
{
public:
	void Initialize();
	void Update();
	void SetupQuad();
	void SetModel(std::shared_ptr<Model> model);
private:
	Shader* shader;
	Texture* texture;

	std::shared_ptr<Model> m_model;
};
