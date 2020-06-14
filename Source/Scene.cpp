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

ArcBall& Scene::GetArcBall()
{
	return m_arcball;
}

Model& Scene::GetModel()
{
	return *m_model;
}

Model& Scene::GetGroundPlane()
{
	return *m_groundPlane;
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
	glm::vec3 modelCenter = model->m_bounds.getCenter();
	m_camera.position.x = modelCenter.x;
	m_camera.position.y = modelCenter.y;
	float maxDimension = model->m_bounds.getMaxDimension();

	m_camera.distance = maxDimension * 2.0f;
	m_camera.position.z = m_camera.distance;

	m_model = model;
}

void Scene::SetGroundPlane(std::shared_ptr<Model> plane)
{
	m_groundPlane = plane;
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
