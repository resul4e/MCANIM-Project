#include "AnimationPlayer.h"
#include "Rig.h"
#include "Joint.h"
#include "Model.h"
#include "Scene.h"
#include "AnimationClip.h"
#include "Channel.h"

AnimationPlayer::AnimationPlayer(std::shared_ptr<Scene> _scene, std::shared_ptr<Model> _model, std::shared_ptr<Rig> _rig) :
	m_scene(_scene),
	m_model(_model),
	m_rig(_rig),
	time(0)
{
	m_scene->SetRig(m_rig);
	m_scene->SetModel(m_model);
}

void AnimationPlayer::AddAnimation(std::shared_ptr<AnimationClip> _anim)
{
	m_animations.push_back(_anim);
}

void AnimationPlayer::Play(float _dt)
{
	const auto anim = m_animations[0];
	
	time += _dt * anim->GetFPS();

	if(time > anim->GetDuration())
	{
		time -= anim->GetDuration();
	}
	
	for(auto j :m_rig->GetAllJoints())
	{
		if(!anim->HasChannel(j->GetName()))
		{
			continue;
		}

		auto channel = anim->GetChannel(j->GetName());
		j->SetLocalTransform(channel->GetValue(time));
	}
}


void AnimationPlayer::Reset()
{
	time = 0;
}
