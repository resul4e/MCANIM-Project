#pragma once

#include "Camera.h"
#include "Model.h"
#include "Rig.h"

class Scene
{
public:
	Scene();
	Camera& GetCamera();
	Model& GetModel();
	Rig& GetRig();
	void SetModel(std::shared_ptr<Model> model);
	void SetRig(std::shared_ptr<Rig> rig);

private:
	Camera m_camera;
	std::shared_ptr<Model> m_model;
	std::shared_ptr<Rig> m_rig;
};
