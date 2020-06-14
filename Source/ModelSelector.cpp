#include "ModelSelector.h"
#include "imgui.h"
#include "Model.h"
#include "FBXLoader.h"
#include <iostream>

ModelSelector::ModelSelector() :
	m_isGuiOpen(true) {
}

void ModelSelector::SetModel(Scene& _scene, std::pair<std::string, std::shared_ptr<Model>> _model) {
	m_currentModel = _model;
	//Also use corresponding rig
	auto it = std::find_if(FBXLoader::fbxRigs.begin(), FBXLoader::fbxRigs.end(),
		[_model](const std::pair<std::string, std::shared_ptr<Rig>> elem) {
			return elem.first == _model.first;
		});
	if (it != FBXLoader::fbxRigs.end()) {
		_scene.SetRig(it->second);
	}
	else {
		std::cout << "Cannot find rig for " << it->first << std::endl;
	}
	_scene.SetModel(m_currentModel.second);
	_scene.GetModel().Upload();
}

void ModelSelector::ImGuiRender(Scene& _scene) {
	if (ImGui::IsKeyPressed('M')) {
		ToggleImguiWindow();
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
				SetModel(_scene, model);
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();  
			}
		}
		ImGui::EndCombo();
	}

	ImGui::End();
}

void ModelSelector::ToggleImguiWindow()
{
	m_isGuiOpen = !m_isGuiOpen;
}
