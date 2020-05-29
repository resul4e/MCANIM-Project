#pragma once
#include <memory>
#include <vector>

class Model;
class Rig;
class Scene;
class AnimationClip;

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
	void Play(float _dt);

	/**
	 * \brief Reset the timer to 0
	 */
	void Reset();

private:

	float time;
	
	std::vector<std::shared_ptr<AnimationClip>> m_animations;
	std::shared_ptr<Rig> m_rig;
	std::shared_ptr<Model> m_model;
	std::shared_ptr<Scene> m_scene;
};
