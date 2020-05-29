#pragma once
#include <memory>
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

class AnimationPlayer
{
public:
	AnimationPlayer() = delete;
	AnimationPlayer(std::shared_ptr<Scene> _scene, std::shared_ptr<Model> _model, std::shared_ptr<Rig> _rig);
	~AnimationPlayer() = default;

	/**
	 * \brief Adds an animation that can be played
	 * \note currently only the first animation is ever played.
	 * \param _anim The animation to be added.
	 */
	void AddAnimation(std::shared_ptr<AnimationClip> _anim);

	/**
	 * \brief Play the selected animation.	
	 * \param _dt How many seconds to advance the playback.
	 */
	void Update(float _dt);

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
	void ImGuiRender();

private:

	float time;

	PlaybackState m_state;
	
	std::shared_ptr<AnimationClip> m_currenAnim;	
	std::vector<std::shared_ptr<AnimationClip>> m_animations;
	std::shared_ptr<Rig> m_rig;
	std::shared_ptr<Model> m_model;
	std::shared_ptr<Scene> m_scene;
};
