#pragma once

#include "Model.h"
#include "Rig.h"

class Scene
{
public:
	Model& GetModel();
	Rig& GetRig();
	void SetModel(std::shared_ptr<Model> model);
	void SetRig(std::shared_ptr<Rig> rig);

private:
	std::shared_ptr<Model> m_model;
	std::shared_ptr<Rig> m_rig;
};
