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

	void setScene(Scene* _scene);

	/**
	 * \brief Renders the Gui for the model selector
	 */
	void ImGuiRender();

	void ToggleImguiWindow();

	bool IsGuiOpen() { return m_isGuiOpen; }
	
private:

	//If we should have the GUI open
	bool m_isGuiOpen;

	//Reference to the scene in order to change the model
	Scene* scene;

	std::pair<std::string, std::shared_ptr<Model>> m_currentModel;
};