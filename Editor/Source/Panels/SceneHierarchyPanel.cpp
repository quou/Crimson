#include "SceneHierarchyPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../FontAwesome.h"

#include "../UIUtils.h"

#include "AssetManagerPanel.h"

SceneHierarchyPanel::SceneHierarchyPanel(Crimson::Scene* scene) : m_scene(scene) {}

template <typename T, typename ComponentDrawFunction>
static void DrawComponent(const std::string& componentName, Crimson::Entity ent, ComponentDrawFunction drawFunction, bool canRemove = true) {
	const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;

	if (ent.HasComponent<T>()) {
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();

		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, "%s", componentName.c_str());
		ImGui::PopStyleVar();

		ImGui::SameLine(contentRegionAvail.x - lineHeight * 0.5f);
		if (ImGui::Button("+", ImVec2(lineHeight, lineHeight))) {
			ImGui::OpenPopup("ComponentSettings");
		}

		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentSettings")) {
			if (ImGui::MenuItem("Remove Component", "", false, canRemove) && canRemove) {
				removeComponent = true;
			}
			ImGui::EndPopup();
		}

		if (open) {
			drawFunction(ent.GetComponent<T>());

			ImGui::TreePop();
		}

		if (removeComponent) {
			ent.RemoveComponent<T>();
		}
	}
}

void SceneHierarchyPanel::Render(AssetManagerPanel& assetManagerPanel) {
	ImGui::Begin("Scene Hierarchy");

	if (ImGui::BeginDragDropTarget()) {
		if (ImGuiPayload const* payload = ImGui::AcceptDragDropPayload("Reparent Entity")) {
			auto draggedEnt = *static_cast<Crimson::Entity*>(payload->Data);

			auto& parent = draggedEnt.GetComponent<Crimson::TransformComponent>().parent;
			if (parent) {
				auto& parentChildren = parent.GetComponent<Crimson::TransformComponent>().children;

				parentChildren.erase(std::remove(parentChildren.begin(), parentChildren.end(), draggedEnt), parentChildren.end());
				parent = Crimson::Entity();
			}
		}
		ImGui::EndDragDropTarget();
	}

	for (auto ent : m_scene->GetEntitiesWithComponent<Crimson::TransformComponent>()) {
		if (!ent.GetComponent<Crimson::TransformComponent>().parent) {
			DrawEntityNode(ent);
		}
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
		if (assetManagerPanel.m_texturePreview) {
			assetManagerPanel.m_texturePreview.reset();
		}

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

				if (ImGui::MenuItem("Particle System")) {
					m_selectedEntity.AddComponent<Crimson::ParticleSystemComponent>(100, 10, 0.5f);
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
	} else if (assetManagerPanel.m_texturePreview) {
		float texWidth = ImGui::GetWindowSize().x-16;
		float texHeight = ImGui::GetWindowSize().x-16;

		ImGui::Text("Texture Preview");

		ImGui::Text("%s", assetManagerPanel.m_texturePreviewPath.c_str());
		ImGui::Text("%dx%d", assetManagerPanel.m_texturePreview->GetSize().first, assetManagerPanel.m_texturePreview->GetSize().second);

		ImVec2 pos = ImGui::GetCursorScreenPos();


		ImGui::Image((ImTextureID)assetManagerPanel.m_texturePreview->GetID(), ImVec2(texWidth, texHeight), ImVec2(0, 0), ImVec2(1, 1));

		ImGuiIO& io = ImGui::GetIO();

		ImVec4 tintColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		ImVec4 borderColor = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);

		if (ImGui::IsItemHovered()){
			 ImGui::BeginTooltip();
			 float region_sz = 32.0f;
			 float region_x = io.MousePos.x - pos.x - region_sz * 0.5f;
			 float region_y = io.MousePos.y - pos.y - region_sz * 0.5f;
			 float zoom = 4.0f;
			 if (region_x < 0.0f) { region_x = 0.0f; }
			 else if (region_x > texWidth - region_sz) { region_x = texWidth - region_sz; }
			 if (region_y < 0.0f) { region_y = 0.0f; }
			 else if (region_y > texHeight - region_sz) { region_y = texHeight - region_sz; }
			 ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
			 ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
			 ImVec2 uv0 = ImVec2((region_x) / texWidth, (region_y) / texHeight);
			 ImVec2 uv1 = ImVec2((region_x + region_sz) / texWidth, (region_y + region_sz) / texHeight);
			 ImGui::Image((ImTextureID)assetManagerPanel.m_texturePreview->GetID(), ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, tintColor, borderColor);
			 ImGui::EndTooltip();
		}
	}
	ImGui::End();
}

void SceneHierarchyPanel::DrawComponents(Crimson::Entity ent) {
	DrawComponent<Crimson::TransformComponent>("Transform", ent, [](auto& component){
		auto& name = component.name;
		auto& tag = component.tag;

		DrawBoolControl("Active", &component.active);

		DrawTextControl("Name", name);
		DrawTextControl("Tag", tag);

		glm::vec3 newRotation = glm::degrees(glm::eulerAngles(component.rotation));

		DrawVec3Control("Translation", component.position);
		if (DrawVec3Control("Rotation", newRotation)) {
			component.rotation = glm::radians(newRotation);
		}
		DrawVec3Control("Scale", component.scale, 1.0f);
	}, false);

	DrawComponent<Crimson::DirectionalLightComponent>("Directional Light", ent, [](auto& component){
		DrawFloatControl("Intensity", &component.intensity, 0.01f);
		DrawColorControl("Color", component.color);
	});

	DrawComponent<Crimson::AmbientLightComponent>("Ambient Light", ent, [](auto& component){
		DrawFloatControl("Intensity", &component.intensity, 0.01f);
		DrawColorControl("Color", component.color);
	});

	DrawComponent<Crimson::PointLightComponent>("Point Light", ent, [](auto& component){
		DrawFloatControl("Intensity", &component.intensity, 0.01f);
		DrawFloatControl("Constant", &component.constant, 0.01f);
		DrawFloatControl("Linear", &component.linear, 0.0001f);
		DrawFloatControl("Quadratic", &component.quadratic, 0.00001f);
		DrawColorControl("Color", component.color);
	});

	DrawComponent<Crimson::MeshFilterComponent>("Mesh Filter", ent, [](auto& component){
		DrawTextLabel("Mesh", component.path);

		if (ImGui::BeginDragDropTarget()) {
			if (ImGuiPayload const* payload = ImGui::AcceptDragDropPayload(".mesh")) {
				std::string toSet = static_cast<const char*>(payload->Data);
				component.path = toSet;
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::SameLine();
		ImGui::PushID("Mesh Reset");
		if (ImGui::Button(ICON_FK_REFRESH)) {
			component.path = "Cube";
		}
		ImGui::PopID();
	});

	DrawComponent<Crimson::MaterialComponent>("Material", ent, [](auto& component){
		DrawTextLabel("Config", component.path);

		if (ImGui::BeginDragDropTarget()) {
			if (ImGuiPayload const* payload = ImGui::AcceptDragDropPayload(".mat")) {
				std::string toSet = static_cast<const char*>(payload->Data);
				component.path = toSet;
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::SameLine();
		ImGui::PushID("Material Reset");
		if (ImGui::Button(ICON_FK_REFRESH)) {
			component.path = "Default";
		}
		ImGui::PopID();
	});

	DrawComponent<Crimson::CameraComponent>("Camera", ent, [](auto& component){
		DrawFloatControl("FOV", &component.camera.m_fov, 0.1f);
		DrawFloatControl("Near", &component.camera.m_near);
		DrawFloatControl("Far", &component.camera.m_far);
		DrawBoolControl("Active", &component.active);
	});

	DrawComponent<Crimson::ParticleSystemComponent>("Particle System", ent, [](auto& component){
		DrawIntControl("Max Particles", &component.maxParticles, 10);
		DrawIntControl("Rate Over Time", &component.rateOverTime);
		DrawFloatControl("Gravity Multiplier", &component.gravity, 0.001f);
		DrawFloatControl("Size Over Lifetime", &component.sizeOverLifetime, 0.001f);
		DrawFloatControl("Max Velocity", &component.randomVelocityMax, 0.001f);
		DrawFloatControl("Min Velocity", &component.randomVelocityMin, 0.001f);
		DrawFloatControl("Max Lifetime", &component.randomLifetimeMax, 0.001f);
		DrawFloatControl("Min Lifetime", &component.randomLifetimeMin, 0.001f);
		DrawFloatControl("Start Size", &component.startSize, 0.001f);
	});

	DrawComponent<Crimson::BoxColliderComponent>("Box Collider", ent, [](auto& component){
		DrawVec3Control("Extents", component.extents);
	});

	DrawComponent<Crimson::SphereColliderComponent>("Sphere Collider", ent, [](auto& component){
		DrawFloatControl("Radius", &component.radius);
	});

	DrawComponent<Crimson::PhysicsComponent>("Physics", ent, [](auto& component){
		DrawBoolControl("Use Gravity", &component.useGravity);
		DrawBoolControl("Kinematic", &component.isKinematic);

		DrawFloatControl("Mass", &component.mass, 0.0f, 0.0f, 0.001f);
		DrawFloatControl("Friction", &component.friction, 0.0f, 1.0f, 0.001f);
		DrawFloatControl("Bounciness", &component.bounciness, 0.0f, 1.0f, 0.001f);
		DrawVec3Control("Center of Gravity", component.cog);
	});

	DrawComponent<Crimson::ScriptComponent>("Script", ent, [](auto& component){
		DrawTextControl("Class Name", component.className);
	});
}

void SceneHierarchyPanel::DrawEntityNode(Crimson::Entity ent) {
	std::string name = ent.GetComponent<Crimson::TransformComponent>().name;

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

	if (ent.GetComponent<Crimson::TransformComponent>().children.size() <= 0) {
		flags |= ImGuiTreeNodeFlags_Leaf;
	}

	if (m_selectedEntity == ent) {
		flags |= ImGuiTreeNodeFlags_Selected;
	}

	std::string titleString = name;
	if (ent.HasComponent<Crimson::MeshFilterComponent>()) {
		titleString = ICON_FK_CUBE + ("   " + name);
	} else if (ent.HasComponent<Crimson::AmbientLightComponent>() || ent.HasComponent<Crimson::PointLightComponent>() || ent.HasComponent<Crimson::DirectionalLightComponent>()) {
		titleString = ICON_FK_LIGHTBULB_O + ("   " + name);
	} else if (ent.HasComponent<Crimson::CameraComponent>()) {
		titleString = ICON_FK_CAMERA + ("   " + name);
	} else if (ent.HasComponent<Crimson::ScriptComponent>()) {
		titleString = ICON_FK_CODE + ("   " + name);
	} else if (ent.HasComponent<Crimson::ParticleSystemComponent>()) {
		titleString = ICON_FK_FIRE + ("   " + name);
	}

	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)ent, flags, "%s", titleString.c_str());

	if (ImGui::IsItemClicked()) {
		if (m_selectedEntity == ent) {
			m_selectedEntity = Crimson::Entity();
		} else {
			m_selectedEntity = ent;
		}
	}

	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
		ImGui::Text("Reparent");

		ImGui::SetDragDropPayload("Reparent Entity", &ent, sizeof(ent));

		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget()) {
		if (ImGuiPayload const* payload = ImGui::AcceptDragDropPayload("Reparent Entity")) {
			auto draggedEnt = *static_cast<Crimson::Entity*>(payload->Data);

			if (draggedEnt != ent && ent.GetComponent<Crimson::TransformComponent>().parent != draggedEnt) {
				if (draggedEnt.GetComponent<Crimson::TransformComponent>().parent) {
					auto& parent = draggedEnt.GetComponent<Crimson::TransformComponent>().parent;
					auto& parentChildren = parent.GetComponent<Crimson::TransformComponent>().children;

					parentChildren.erase(std::remove(parentChildren.begin(), parentChildren.end(), draggedEnt), parentChildren.end());
				}

				draggedEnt.GetComponent<Crimson::TransformComponent>().parent = ent;
				ent.GetComponent<Crimson::TransformComponent>().children.push_back(draggedEnt);
			}
		}
		ImGui::EndDragDropTarget();
	}

	bool entityDeleted = false;
	if (ImGui::BeginPopupContextItem()) {
		if (ImGui::MenuItem("Delete")) {
			entityDeleted = true;
		}

		if (ImGui::MenuItem("Unparent")) {
			auto& parent = ent.GetComponent<Crimson::TransformComponent>().parent;
			if (parent) {
				auto& children = parent.GetComponent<Crimson::TransformComponent>().children;
				children.erase(std::remove(children.begin(), children.end(), ent), children.end());
				parent = Crimson::Entity();
			}
		}

		ImGui::EndPopup();
	}

	if (opened) {
		for (auto child : ent.GetComponent<Crimson::TransformComponent>().children) {
			DrawEntityNode(child);
		}

		ImGui::TreePop();
	}

	if (entityDeleted) {
		m_scene->DestroyEntity(ent);
		if (m_selectedEntity == ent) {
			m_selectedEntity = Crimson::Entity();
		}
	}
}
