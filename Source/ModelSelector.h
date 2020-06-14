#pragma once
#include <memory>
#include <string>
#include "Scene.h"

class Model;
/**
 * \brief Allows changing of the character's model
 */
class ModelSelector {
public:
	ModelSelector();
	~ModelSelector() = default;

	void SetModel(Scene & _scene, std::pair<std::string, std::shared_ptr<Model>> _model);

	/**
	 * \brief Renders the Gui for the model selector
	 */
	void ImGuiRender(Scene& _scene);

	void ToggleImguiWindow();

	bool IsGuiOpen() { return m_isGuiOpen; }
	
private:

	//If we should have the GUI open
	bool m_isGuiOpen;

	std::pair<std::string, std::shared_ptr<Model>> m_currentModel;
};