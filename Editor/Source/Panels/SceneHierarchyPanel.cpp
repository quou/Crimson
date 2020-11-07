#include "SceneHierarchyPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f) {
	ImGui::PushID(label.c_str());

	ImGui::Columns(2, NULL, false);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text("%s", label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	if (ImGui::Button("X", buttonSize))
		values.x = resetValue;
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	if (ImGui::Button("Y", buttonSize))
		values.y = resetValue;
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	if (ImGui::Button("Z", buttonSize))
		values.z = resetValue;
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1, NULL, false);

	ImGui::PopID();
}

static void DrawTextControl(const std::string& label, std::string& string, float colWidth = 100.0f) {
	ImGui::PushID(label.c_str());

	ImGui::Columns(2, NULL, false);


	ImGui::SetColumnWidth(0, colWidth);
	ImGui::Text("%s", label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer, string.c_str());
	if (ImGui::InputText("##STRIN", buffer, sizeof(buffer))) {
		string = std::string(buffer);
	}

	ImGui::PopItemWidth();

	ImGui::Columns(1, NULL, false);

	ImGui::PopID();
}

static void DrawFloatControl(const std::string& label, float* val, float step = 1.0f, float colWidth = 100.0f) {
	ImGui::PushID(label.c_str());

	ImGui::Columns(2, NULL, false);


	ImGui::SetColumnWidth(0, colWidth);
	ImGui::Text("%s", label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
	ImGui::DragFloat("##FLIN", val, step);

	ImGui::PopItemWidth();

	ImGui::Columns(1, NULL, false);

	ImGui::PopID();
}

static void DrawColorControl(const std::string& label, glm::vec3& value, float colWidth = 100.0f) {
	ImGui::PushID(label.c_str());

	ImGui::Columns(2, NULL, false);


	ImGui::SetColumnWidth(0, colWidth);
	ImGui::Text("%s", label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
	ImGui::ColorEdit3("##COLIN", glm::value_ptr(value));

	ImGui::PopItemWidth();

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

			DrawVec3Control("Translation", transform.position);
			DrawVec3Control("Rotation", transform.rotation);
			DrawVec3Control("Scale", transform.scale, 1.0f);

			ImGui::TreePop();
		}
	}

	if (ent.HasComponent<Crimson::DirectionalLightComponent>()) {
		if (ImGui::TreeNodeEx((void*)typeid(Crimson::DirectionalLightComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Directional Light")) {

			DrawFloatControl("Intensity", &ent.GetComponent<Crimson::DirectionalLightComponent>().intensity, 0.01f);
			DrawColorControl("Color", ent.GetComponent<Crimson::DirectionalLightComponent>().color);

			ImGui::TreePop();
		}
	}

	if (ent.HasComponent<Crimson::AmbientLightComponent>()) {
		if (ImGui::TreeNodeEx((void*)typeid(Crimson::AmbientLightComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Ambient Light")) {

			DrawFloatControl("Intensity", &ent.GetComponent<Crimson::AmbientLightComponent>().intensity, 0.01f);
			DrawColorControl("Color", ent.GetComponent<Crimson::AmbientLightComponent>().color);

			ImGui::TreePop();
		}
	}

	if (ent.HasComponent<Crimson::PointLightComponent>()) {
		if (ImGui::TreeNodeEx((void*)typeid(Crimson::DirectionalLightComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Point Light")) {

			DrawFloatControl("Intensity", &ent.GetComponent<Crimson::PointLightComponent>().intensity, 0.01f);
			DrawFloatControl("Constant", &ent.GetComponent<Crimson::PointLightComponent>().constant, 0.01f);
			DrawFloatControl("Linear", &ent.GetComponent<Crimson::PointLightComponent>().linear, 0.0001f);
			DrawFloatControl("Quadratic", &ent.GetComponent<Crimson::PointLightComponent>().quadratic, 0.00001f);
			DrawColorControl("Color", ent.GetComponent<Crimson::PointLightComponent>().color);

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
