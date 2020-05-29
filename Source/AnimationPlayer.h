#pragma once
#include <memory>

class Model;
class Rig;
class Scene;

class AnimationPlayer
{
public:
	AnimationPlayer() = delete;
	AnimationPlayer(std::shared_ptr<Scene> _scene, std::shared_ptr<Model> _model, std::shared_ptr<Rig> _rig);
	~AnimationPlayer();

private:

	void ResetPlayerRig();
	
	std::shared_ptr<Rig> m_playerRig;
	std::shared_ptr<Rig> m_originalRig;
	std::shared_ptr<Model> m_model;
	std::shared_ptr<Scene> m_scene;
};
