#include "AnimationPlayer.h"
#include "Rig.h"
#include "Joint.h"
#include "Model.h"
#include "Scene.h"
#include "AnimationClip.h"
#include "Channel.h"
#include "imgui.h"

AnimationPlayer::AnimationPlayer(std::shared_ptr<Scene> _scene, std::shared_ptr<Model> _model, std::shared_ptr<Rig> _rig) :
	m_scene(_scene),
	m_model(_model),
	m_rig(_rig),
	time(0),
	m_state(PlaybackState::STOPPED)
{
	m_scene->SetRig(m_rig);
	m_scene->SetModel(m_model);
}

void AnimationPlayer::AddAnimation(std::shared_ptr<AnimationClip> _anim)
{
	if (m_animations.size() == 0)
	{
		m_currenAnim = _anim;
	}
	
	m_animations.push_back(_anim);

	
}

void AnimationPlayer::Update(float _dt)
{
	if(m_state == PlaybackState::PLAYING)
	{
		//Advance and reset timer if necessary.
		time += _dt * m_currenAnim->GetFPS();
		if (time > m_currenAnim->GetDuration())
		{
			time -= m_currenAnim->GetDuration();
		}
	}
	
	for(auto j :m_rig->GetAllJoints())
	{
		if(!m_currenAnim->HasChannel(j->GetName()))
		{
			continue;
		}

		auto channel = m_currenAnim->GetChannel(j->GetName());
		j->SetLocalTransform(channel->GetValue(time));
	}
}

void AnimationPlayer::Play()
{
	m_state = PlaybackState::PLAYING;
}

void AnimationPlayer::Pause()
{
	m_state = PlaybackState::PAUSED;
}

void AnimationPlayer::Stop()
{
	Reset();
	m_state = PlaybackState::STOPPED;
}


void AnimationPlayer::Reset()
{
	time = 0;
}

void AnimationPlayer::ImGuiRender()
{
	ImGui::Begin("AnimationPlayer");
	
	//Play/pause button.
	bool playing = m_state == PlaybackState::PLAYING;
	ImGui::Checkbox(playing ? "Pause" : "Play", &playing);
	if(playing)
	{
		Play();
	}
	else if(m_state == PlaybackState::PLAYING && !playing)
	{
		Pause();
	}

	//Stop button.
	bool stop = false;
	ImGui::Checkbox("Stop", &stop);
	if(stop)
	{
		Stop();
	}

	//Time slider.
	auto prevTime = time;
	ImGui::SliderFloat("Time", &time, 0, m_currenAnim->GetDuration());
	if(time != prevTime)
	{
		Pause();
	}
	
	ImGui::End();
}
