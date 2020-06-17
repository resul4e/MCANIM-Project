#include "AnimationPlayer.h"
#include "AnimationLoader.h"
#include "Rig.h"
#include "Joint.h"
#include "Model.h"
#include "Scene.h"
#include "AnimationClip.h"
#include "Channel.h"
#include "imgui.h"
#include "FBXLoader.h"
#include "Options.h"

AnimationPlayer::AnimationPlayer() :
	time(0),
	m_state(PlaybackState::STOPPED),
	m_isGuiOpen(true),
	m_skinningMethod(SkinningMethod::DUAL_QUATERNION)
{
}

void AnimationPlayer::AddAnimation(std::shared_ptr<AnimationClip> _anim)
{
	if (m_animations.empty())
	{
		m_currentAnim = _anim;
	}
	
	m_animations.push_back(_anim);
}

void AnimationPlayer::Update(Scene& scene, float _dt)
{
	if(m_state == PlaybackState::PLAYING)
	{
		//Advance and reset timer if necessary.
		time += _dt * playbackSpeed * m_currentAnim->GetFPS();
		if (time > m_currentAnim->GetDuration())
		{
			time -= m_currentAnim->GetDuration();
		}
	}
	
	for(auto j : scene.GetRig().GetAllJoints())
	{
		if(!m_currentAnim->HasChannel(j->GetName()))
		{
			continue;
		}

		auto channel = m_currentAnim->GetChannel(j->GetName());
		j->SetLocalTransform(channel->GetValue(time));
	}

	// Perform skinning on the model
	scene.GetModel().UpdateVertices(scene.GetRig(), m_skinningMethod);
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
	if(ImGui::IsKeyPressed('G' /*G*/))
	{
		ToggleImguiWindow();
	}
	
	if(!m_isGuiOpen)
	{
		return;
	}
	
	ImGui::Begin("AnimationPlayer", &m_isGuiOpen);

	//Dropdown with all animations that can be chosen
	const std::string currentSelection = m_currentAnim->GetName();
	if(ImGui::BeginCombo("Animation", currentSelection.c_str()))
	{
		for (auto anim : m_animations)
		{
			bool is_selected = strcmp(currentSelection.c_str(), anim->GetName().c_str()) == 0; // You can store your selection however you want, outside or inside your objects
			if (ImGui::Selectable(anim->GetName().c_str(), is_selected))
			{
				m_currentAnim = anim;
				Reset();
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
			}
		}
		ImGui::EndCombo();
	}

	//Animation loading button
	if (ImGui::Button("Rescan Animations")) {
		FBXLoader::ScanNew();
		for (std::shared_ptr<AnimationClip> animation : FBXLoader::fbxAnimations) {
			if (std::find(m_animations.begin(), m_animations.end(), animation) == m_animations.end()){
				AddAnimation(animation);
			}
		}
	}

	// Dropdown for skinning algorithms
	{
		const std::string currentSelection = m_skinningMethod == SkinningMethod::DUAL_QUATERNION ? "Dual Quaternion" : "Linear Blend";
		std::vector<std::string> options{ "Dual Quaternion", "Linear Blend" };
		if (ImGui::BeginCombo("Skin blending", currentSelection.c_str()))
		{
			for (auto anim : options)
			{
				bool is_selected = currentSelection == anim;
				if (ImGui::Selectable(anim.c_str(), is_selected))
				{
					if (anim == "Dual Quaternion")
						m_skinningMethod = SkinningMethod::DUAL_QUATERNION;
					else if (anim == "Linear Blend")
						m_skinningMethod = SkinningMethod::LINEAR_BLEND;

					Reset();
				}
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}

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
	ImGui::SliderFloat("Time", &time, 0, m_currentAnim->GetDuration());
	if(time != prevTime)
	{
		Pause();
	}

	// Playback speed slider
	ImGui::SliderFloat("Playback Speed", &playbackSpeed, 0, 1);

	// Model toggle
	ImGui::Checkbox("Enable Model", &Options::RenderModel);

	// Texture toggle
	ImGui::Checkbox("Enable Textures", &Options::RenderTextures);

	ImGui::End();
}

void AnimationPlayer::ToggleImguiWindow()
{
	m_isGuiOpen = !m_isGuiOpen;
}

void AnimationPlayer::SwitchSkinning()
{
	switch (m_skinningMethod)
	{
	case SkinningMethod::LINEAR_BLEND:
		m_skinningMethod = SkinningMethod::DUAL_QUATERNION;
		break;
	case SkinningMethod::DUAL_QUATERNION:
		m_skinningMethod = SkinningMethod::LINEAR_BLEND;
		break;
	default:
		throw std::exception("Please implement switch to other skinning method!");
		break;
	}
}
