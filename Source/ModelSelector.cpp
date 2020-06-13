#include "ModelSelector.h"
#include "imgui.h"
#include "Model.h"
#include "FBXLoader.h"

ModelSelector::ModelSelector() :
	m_isGuiOpen(true) {
}


void ModelSelector::setScene(Scene* _scene) {
	scene = _scene;
}

void ModelSelector::ImGuiRender() {
	if (ImGui::IsKeyPressed(71 /*G*/)) {
		m_isGuiOpen = !m_isGuiOpen;
	}

	if (!m_isGuiOpen) {
		return;
	}

	ImGui::Begin("ModelSelector", &m_isGuiOpen);

	//Dropdown with all models that can be chosen
	const std::string currentSelection = m_currentModel.first;
	if (ImGui::BeginCombo("Model", currentSelection.c_str())) {
		for (std::pair<std::string, std::shared_ptr<Model>> model : FBXLoader::fbxModels) {
			bool is_selected = strcmp(currentSelection.c_str(), model.first.c_str()) == 0;
			if (ImGui::Selectable(model.first.c_str(), is_selected)) {
				m_currentModel = model;
				scene->SetModel(m_currentModel.second);
				scene->GetModel().Upload();
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();  
			}
		}
		ImGui::EndCombo();
	}

	ImGui::End();
}