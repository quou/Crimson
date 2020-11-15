#include "SceneHierarchyPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include "../FontAwesome.h"

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

static void DrawFloatControl(const std::string& label, float* val, float min=0.0f, float max=0.0f, float step = 1.0f, float colWidth = 100.0f) {
	ImGui::PushID(label.c_str());

	ImGui::Columns(2, NULL, false);


	ImGui::SetColumnWidth(0, colWidth);
	ImGui::Text("%s", label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
	ImGui::DragFloat("##FLIN", val, step, min, max);

	ImGui::PopItemWidth();

	ImGui::Columns(1, NULL, false);

	ImGui::PopID();
}

static void DrawBoolControl(const std::string& label, bool* val, float colWidth = 100.0f) {
	ImGui::PushID(label.c_str());

	ImGui::Columns(2, NULL, false);


	ImGui::SetColumnWidth(0, colWidth);
	ImGui::Text("%s", label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
	ImGui::Checkbox("##BIN", val);

	ImGui::PopItemWidth();

	ImGui::Columns(1, NULL, false);

	ImGui::PopID();
}

static void DrawTextLabel(const std::string& label, const std::string& text, float colWidth = 100.0f) {
	ImGui::PushID(label.c_str());

	ImGui::Columns(2, NULL, false);

	ImGui::SetColumnWidth(0, colWidth);
	ImGui::Text("%s", label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
	ImGui::Text("%s", text.c_str());

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

	// Right click on blank space
	if (ImGui::BeginPopupContextWindow(0, 1, false)) {
		if (ImGui::BeginMenu("Create...")) {
			if (ImGui::MenuItem("Empty Entity")) {
				m_scene->CreateEntity("New Entity");
			}

			if (ImGui::MenuItem("Cube")) {
				auto e = m_scene->CreateEntity("Cube");
				e.AddComponent<Crimson::MeshFilterComponent>("Cube");
				e.AddComponent<Crimson::MaterialComponent>("Default");
			}

			if (ImGui::MenuItem("Physics Cube")) {
				auto e = m_scene->CreateEntity("Physics Cube");
				e.AddComponent<Crimson::MeshFilterComponent>("Cube");
				e.AddComponent<Crimson::MaterialComponent>("Default");
				e.AddComponent<Crimson::BoxColliderComponent>(glm::vec3(1.0f));
				e.AddComponent<Crimson::PhysicsComponent>(true, 1.0f, 0.3f, 0.1f, false, glm::vec3(0));
			}

			if (ImGui::MenuItem("Sun")) {
				auto e = m_scene->CreateEntity("Sun");
				e.AddComponent<Crimson::DirectionalLightComponent>(glm::vec3(1,1,1), 1.0f);
				e.AddComponent<Crimson::AmbientLightComponent>(glm::vec3(1,1,1), 0.1f);
			}

			if (ImGui::MenuItem("Lamp")) {
				auto e = m_scene->CreateEntity("Lamp");
				e.AddComponent<Crimson::PointLightComponent>(1.0f, 0.09f, 0.032f, glm::vec3(1,1,1), 1.0f);
			}

			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}

	ImGui::End();

	ImGui::Begin("Properties");
	if (m_selectedEntity) {
		DrawComponents(m_selectedEntity);

		if (ImGui::Button("Add Component")) {
			ImGui::OpenPopup("AddComponent");
		}

		if (ImGui::BeginPopup("AddComponent")) {
			if (ImGui::BeginMenu("Lights")) {
				if (ImGui::MenuItem("Point Light")) {
					m_selectedEntity.AddComponent<Crimson::PointLightComponent>(1.0f, 0.09f, 0.032f, glm::vec3(1,1,1), 1.0f);
				}

				if (ImGui::MenuItem("Ambient Light")) {
					m_selectedEntity.AddComponent<Crimson::AmbientLightComponent>(glm::vec3(1,1,1), 0.1f);
				}

				if (ImGui::MenuItem("Directional Light")) {
					m_selectedEntity.AddComponent<Crimson::DirectionalLightComponent>(glm::vec3(1,1,1), 1.0f);
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Rendering")) {
				if (ImGui::MenuItem("Mesh Filter")) {
					m_selectedEntity.AddComponent<Crimson::MeshFilterComponent>("Cube");
				}

				if (ImGui::MenuItem("Material")) {
					m_selectedEntity.AddComponent<Crimson::MaterialComponent>("Default");
				}

				if (ImGui::MenuItem("Camera")) {
					m_selectedEntity.AddComponent<Crimson::CameraComponent>(std::pair<int, int>{1366, 768}, 45.0f, 0.01f, 1000.0f, true);
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Physics")) {
				if (ImGui::MenuItem("Physics")) {
					m_selectedEntity.AddComponent<Crimson::PhysicsComponent>(true, 1.0f, 0.3f, 0.1f, false, glm::vec3(0));
				}

				if (ImGui::MenuItem("Box Collider")) {
					m_selectedEntity.AddComponent<Crimson::BoxColliderComponent>(glm::vec3(1));
				}

				if (ImGui::MenuItem("Sphere Collider")) {
					m_selectedEntity.AddComponent<Crimson::SphereColliderComponent>(1.0f);
				}

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Script")) {
				m_selectedEntity.AddComponent<Crimson::ScriptComponent>("-");
			}

			ImGui::EndPopup();
		}
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
			ImGui::Separator();
		}
	}


	if (ent.HasComponent<Crimson::DirectionalLightComponent>()) {
		if (ImGui::TreeNodeEx((void*)typeid(Crimson::DirectionalLightComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Directional Light")) {

			DrawFloatControl("Intensity", &ent.GetComponent<Crimson::DirectionalLightComponent>().intensity, 0.01f);
			DrawColorControl("Color", ent.GetComponent<Crimson::DirectionalLightComponent>().color);

			ImGui::TreePop();
			ImGui::Separator();
		}
	}

	if (ent.HasComponent<Crimson::AmbientLightComponent>()) {
		if (ImGui::TreeNodeEx((void*)typeid(Crimson::AmbientLightComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Ambient Light")) {

			DrawFloatControl("Intensity", &ent.GetComponent<Crimson::AmbientLightComponent>().intensity, 0.01f);
			DrawColorControl("Color", ent.GetComponent<Crimson::AmbientLightComponent>().color);

			ImGui::TreePop();
			ImGui::Separator();
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
			ImGui::Separator();
		}
	}

	if (ent.HasComponent<Crimson::MeshFilterComponent>()) {
		if (ImGui::TreeNodeEx((void*)typeid(Crimson::DirectionalLightComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Mesh Filter")) {
			auto& mesh = ent.GetComponent<Crimson::MeshFilterComponent>();

			DrawTextLabel("Mesh", mesh.path);

			if (ImGui::BeginDragDropTarget()) {
            if (ImGuiPayload const* payload = ImGui::AcceptDragDropPayload(".mesh")) {
               std::string toSet = static_cast<const char*>(payload->Data);
               mesh.path = toSet;
            }
            ImGui::EndDragDropTarget();
         }

			ImGui::TreePop();
			ImGui::Separator();
		}
	}

	if (ent.HasComponent<Crimson::MaterialComponent>()) {
		if (ImGui::TreeNodeEx((void*)typeid(Crimson::DirectionalLightComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Material")) {
			auto& mat = ent.GetComponent<Crimson::MaterialComponent>();

			DrawTextLabel("Config", mat.path);

			if (ImGui::BeginDragDropTarget()) {
            if (ImGuiPayload const* payload = ImGui::AcceptDragDropPayload(".mat")) {
               std::string toSet = static_cast<const char*>(payload->Data);
               mat.path = toSet;
            }
            ImGui::EndDragDropTarget();
         }

			ImGui::TreePop();
			ImGui::Separator();
		}
	}

	if (ent.HasComponent<Crimson::CameraComponent>()) {
		if (ImGui::TreeNodeEx((void*)typeid(Crimson::CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera")) {
			auto& cam = ent.GetComponent<Crimson::CameraComponent>();

			DrawFloatControl("FOV", &cam.camera.m_fov, 0.1f);
			DrawFloatControl("Near", &cam.camera.m_near);
			DrawFloatControl("Far", &cam.camera.m_far);
			DrawBoolControl("Active", &cam.active);

			ImGui::TreePop();
			ImGui::Separator();
		}
	}

	if (ent.HasComponent<Crimson::BoxColliderComponent>()) {
		if (ImGui::TreeNodeEx((void*)typeid(Crimson::CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Box Collider")) {
			auto& box = ent.GetComponent<Crimson::BoxColliderComponent>();

			DrawVec3Control("Extents", box.extents);

			ImGui::TreePop();
			ImGui::Separator();
		}
	}

	if (ent.HasComponent<Crimson::SphereColliderComponent>()) {
		if (ImGui::TreeNodeEx((void*)typeid(Crimson::CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sphere Collider")) {
			auto& sphere = ent.GetComponent<Crimson::SphereColliderComponent>();

			DrawFloatControl("Radius", &sphere.radius);

			ImGui::TreePop();
			ImGui::Separator();
		}
	}

	if (ent.HasComponent<Crimson::PhysicsComponent>()) {
		if (ImGui::TreeNodeEx((void*)typeid(Crimson::CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Physics")) {
			auto& physics = ent.GetComponent<Crimson::PhysicsComponent>();

			DrawBoolControl("Use Gravity", &physics.useGravity);
			DrawBoolControl("Kinematic", &physics.isKinematic);

			DrawFloatControl("Mass", &physics.mass, 0.0f, 0.0f, 0.001f);
			DrawFloatControl("Friction", &physics.friction, 0.0f, 1.0f, 0.001f);
			DrawFloatControl("Bounciness", &physics.bounciness, 0.0f, 1.0f, 0.001f);
			DrawVec3Control("Center of Gravity", physics.cog);

			ImGui::TreePop();
			ImGui::Separator();
		}
	}

	if (ent.HasComponent<Crimson::ScriptComponent>()) {
		if (ImGui::TreeNodeEx((void*)typeid(Crimson::CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Script")) {
			auto& script = ent.GetComponent<Crimson::ScriptComponent>();

			DrawTextControl("Class Name", script.className);

			ImGui::TreePop();
			ImGui::Separator();
		}
	}
}

void SceneHierarchyPanel::DrawEntityNode(Crimson::Entity ent) {
	std::string name = ent.GetComponent<Crimson::TransformComponent>().name;

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

	if (m_selectedEntity == ent) {
		flags |= ImGuiTreeNodeFlags_Selected;
	}
	bool opened;

	if (ent.HasComponent<Crimson::MeshFilterComponent>()) {
		opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)ent, flags, ICON_FK_CUBE "  %s", name.c_str());
	} else if (ent.HasComponent<Crimson::AmbientLightComponent>() || ent.HasComponent<Crimson::PointLightComponent>() || ent.HasComponent<Crimson::DirectionalLightComponent>()) {
		opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)ent, flags, ICON_FK_LIGHTBULB_O "  %s", name.c_str());
	} else if (ent.HasComponent<Crimson::CameraComponent>()) {
		opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)ent, flags, ICON_FK_CAMERA "  %s", name.c_str());
	} else if (ent.HasComponent<Crimson::ScriptComponent>()) {
		opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)ent, flags, ICON_FK_CODE "  %s", name.c_str());
	} else {
		opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)ent, flags, "%s", name.c_str());
	}

	if (ImGui::IsItemClicked()) {
		if (m_selectedEntity == ent) {
			m_selectedEntity = Crimson::Entity();
		} else {
			m_selectedEntity = ent;
		}
	}

	bool entityDeleted = false;
	if (ImGui::BeginPopupContextItem()) {
		if (ImGui::MenuItem("Destroy Entity")) {
			entityDeleted = true;
		}

		ImGui::EndPopup();
	}

	if (opened) {
		ImGui::TreePop();
	}

	if (entityDeleted) {
		m_scene->DestroyEntity(ent);
		if (m_selectedEntity == ent) {
			m_selectedEntity = Crimson::Entity();
		}
	}
}
