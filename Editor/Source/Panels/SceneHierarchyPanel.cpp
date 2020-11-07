#include "SceneHierarchyPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float colWidth = 100.0f) {
	ImGui::PushID(label.c_str());

	ImGui::Columns(2, NULL, false);

	ImGui::SetColumnWidth(0, colWidth);
	ImGui::Text("%s", label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
	if (ImGui::Button("X", buttonSize)) {
		values.x = resetValue;
	}
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, 0.1f);
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
	if (ImGui::Button("Y", buttonSize)) {
		values.y = resetValue;
	}
	ImGui::PopStyleColor(3);


	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f);
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
	if (ImGui::Button("Z", buttonSize)) {
		values.z = resetValue;
	}
	ImGui::PopStyleColor(3);


	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f);
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PopStyleVar();

	ImGui::Columns(1, NULL, false);

	ImGui::PopID();
}

static void DrawTextControl(const std::string& label, std::string& string) {
	ImGui::PushID(label.c_str());

	ImGui::Columns(2, NULL, false);

	ImGui::SetColumnWidth(0, 100.0f);
	ImGui::Text("%s", label.c_str());
	ImGui::NextColumn();

	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer, string.c_str());
	if (ImGui::InputText("##STRIN", buffer, sizeof(buffer))) {
		string = std::string(buffer);
	}

	ImGui::Columns(1, NULL, false);

	ImGui::PopID();
}

SceneHierarchyPanel::SceneHierarchyPanel(Crimson::Scene* scene) : m_scene(scene) {}

void SceneHierarchyPanel::Render() {
	ImGui::Begin("Scene Hierarchy");

	for (auto ent : m_scene->GetEntitiesWithComponent<Crimson::TransformComponent>()) {
		DrawEntityNode(ent);
	}

	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
		m_selectedEntity = Crimson::Entity();
	}

	ImGui::End();

	ImGui::Begin("Properties");
	if (m_selectedEntity) {
		DrawComponents(m_selectedEntity);
	}
	ImGui::End();
}

void SceneHierarchyPanel::DrawComponents(Crimson::Entity ent) {
	if (ent.HasComponent<Crimson::TransformComponent>()) {
		if (ImGui::TreeNodeEx((void*)typeid(Crimson::TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
			auto& name = ent.GetComponent<Crimson::TransformComponent>().name;

			DrawTextControl("Name", name);

			auto& transform = ent.GetComponent<Crimson::TransformComponent>();

			// Position
			DrawVec3Control("Translation", transform.position);
			DrawVec3Control("Rotation", transform.rotation);
			DrawVec3Control("Scale", transform.scale, 1.0f);

			ImGui::TreePop();
		}
	}
}

void SceneHierarchyPanel::DrawEntityNode(Crimson::Entity ent) {
	std::string name = ent.GetComponent<Crimson::TransformComponent>().name;

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

	if (m_selectedEntity == ent) {
		flags |= ImGuiTreeNodeFlags_Selected;
	}

	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)ent, flags, "%s", name.c_str());
	if (ImGui::IsItemClicked()) {
		if (m_selectedEntity == ent) {
			m_selectedEntity = Crimson::Entity();
		} else {
			m_selectedEntity = ent;
		}
	}

	if (opened) {
		ImGui::TreePop();
	}
}
