#include "Scene.h"

Model& Scene::GetModel()
{
	return *m_model;
}

Rig& Scene::GetRig()
{
	return *m_rig;
}

void Scene::SetModel(std::shared_ptr<Model> model)
{
	m_model = model;
}

void Scene::SetRig(std::shared_ptr<Rig> rig)
{
	m_rig = rig;
}
