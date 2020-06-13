#include "Scene.h"

#include <glm/glm.hpp>
#include <glm/gtx/component_wise.hpp>

Scene::Scene() :
	m_camera(glm::radians(60.0f), 1.0, 1.0f, 10000.0f)
{

}

Camera& Scene::GetCamera()
{
	return m_camera;
}

Model& Scene::GetModel()
{
	return *m_model;
}

Rig& Scene::GetRig()
{
	return *m_rig;
}

Texture& Scene::GetTexture()
{
	return *m_texture;
}

Texture& Scene::GetSpecularMap()
{
	return *m_specularMap;
}

void Scene::SetModel(std::shared_ptr<Model> model)
{
	m_camera.position.x = (model->maxBounds.x + model->minBounds.x) / 2;
	m_camera.position.y = (model->maxBounds.y + model->minBounds.y) / 2;
	glm::vec3 bounds = model->maxBounds - model->minBounds;
	float dist = glm::compMax(bounds);
	m_camera.position.z = dist * 1.5f;

	m_model = model;
}

void Scene::SetRig(std::shared_ptr<Rig> rig)
{
	m_rig = rig;
}

void Scene::SetTexture(std::shared_ptr<Texture> texture)
{
	m_texture = texture;
}

void Scene::SetSpecularMap(std::shared_ptr<Texture> texture)
{
	m_specularMap = texture;
}
