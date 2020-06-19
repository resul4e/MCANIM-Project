#pragma once

#include "SkinningMethod.h"

#include <memory>
#include <string>
#include <vector>

class Model;
class Rig;
class Scene;
class AnimationClip;


/**
 * \brief The states the playback of the animation can be in.
 */
enum class PlaybackState
{
	PLAYING,		///< The animation is playing
	STOPPED,		///< The animation is stopped (time is at 0)
	PAUSED			///< The animation is paused (time will resume from wherever it is set)
};

/**
 * \brief This class takes care of playing the selected animation. It can hold multiple animations that all share the same rig and model.
 */
class AnimationPlayer
{
public:
	AnimationPlayer();
	~AnimationPlayer() = default;

	/**
	 * \brief Adds an animation that can be played
	 * \note currently only the first animation is ever played.
	 * \param _anim The animation to be added.
	 */
	void AddAnimation(std::shared_ptr<AnimationClip> _anim);

	void SetModel(Scene& _scene, std::pair<std::string, std::shared_ptr<Model>> _model);

	/**
	 * \brief Play the selected animation.	
	 * \param _dt How many seconds to advance the playback.
	 */
	void Update(Scene& scene, float _dt);

	void Play();
	void Pause();
	void Stop();

	/**
	 * \brief Reset the timer to 0
	 */
	void Reset();

	/**
	 * \brief Renders the Gui for the animation player.
	 */
	void ImGuiRender(Scene& scene);

	/**
	 * \brief Show the imgui window if it is closed, otherwise close it.
	 */
	void ToggleImguiWindow();

	/**
	 * \brief Returns if the imgui window is currently open.
	 */
	bool IsGuiOpen() const { return m_isGuiOpen; };
	
	/**
	 * \brief Toggles between Dual Quaternion mode and Linear Blending mode.
	 */
	void SwitchSkinning();
	
	/**
	 * \brief Returns what the current Skinning Mode is. 
	 */
	SkinningMethod GetSkinningMethod() const { return m_skinningMethod; }

private:

	//The current time of the playback. Will be reset to 0 once the animation has been fully played.
	float time;
	
	// The speed multiplier of the playback
	float playbackSpeed;

	//What the current state of the playback is.
	PlaybackState m_state;
	
	//If we should have the GUI open
	bool m_isGuiOpen;

	// The current model that is shown
	std::pair<std::string, std::shared_ptr<Model>> m_currentModel;

	//The current animation that is being played
	std::shared_ptr<AnimationClip> m_currentAnim;
	//A list of all animations
	std::vector<std::shared_ptr<AnimationClip>> m_animations;
	
	//The method by which we smooth skin the mesh to the bones
	SkinningMethod m_skinningMethod;
};
