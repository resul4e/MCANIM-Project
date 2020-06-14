#pragma once

#include "Camera.h"
#include "Model.h"
#include "Rig.h"
#include "Texture.h"
#include "ArcBall.h"

class Scene
{
public:
	Scene();
	Camera& GetCamera();
	ArcBall& GetArcBall();
	Model& GetModel();
	Model& GetGroundPlane();
	std::shared_ptr<Rig> GetRig();
	std::shared_ptr<Texture> GetTexture();
	std::shared_ptr<Texture> GetSpecularMap();
	void SetModel(std::shared_ptr<Model> model);
	void SetGroundPlane(std::shared_ptr<Model> plane);
	void SetRig(std::shared_ptr<Rig> rig);
	void SetTexture(std::shared_ptr<Texture> texture);
	void SetSpecularMap(std::shared_ptr<Texture> texture);

private:
	Camera m_camera;
	ArcBall m_arcball;
	std::shared_ptr<Model> m_model;
	std::shared_ptr<Rig> m_rig;
	std::shared_ptr<Texture> m_texture;
	std::shared_ptr<Texture> m_specularMap;
	std::shared_ptr<Model> m_groundPlane;
};
