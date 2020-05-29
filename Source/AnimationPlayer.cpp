#include "AnimationPlayer.h"
#include "Rig.h"
#include "Joint.h"
#include "Model.h"
#include "Scene.h"

AnimationPlayer::AnimationPlayer(std::shared_ptr<Scene> _scene, std::shared_ptr<Model> _model, std::shared_ptr<Rig> _rig) :
	m_scene(_scene),
	m_model(_model),
	m_originalRig(_rig),
	m_playerRig(nullptr)
{
	ResetPlayerRig();
	
	m_scene->SetRig(m_playerRig);
	m_scene->SetModel(m_model);
}

AnimationPlayer::~AnimationPlayer()
{
}

void AnimationPlayer::ResetPlayerRig()
{
	m_playerRig = m_originalRig;
}
