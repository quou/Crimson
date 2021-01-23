#include <imgui.h>
#include <imgui_internal.h>

#include "heirarchy.h"

namespace Crimson {
	void Heirarchy::DrawEntityNode(const ref<Entity>& ent) {
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

		if (m_selectionContext == ent.get()) {
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		bool open = ImGui::TreeNodeEx((void*)(uint64_t)ent->GetID(), flags, "%s", ent->m_name.c_str());

		if (ImGui::IsItemClicked()) {
			m_selectionContext = ent.get();
		}

		if (open) {
			ImGui::TreePop();
		}
	}


	template <typename T, typename ComponentDrawFunction>
	static void DrawComponent(const std::string& componentName, Entity* ent, ComponentDrawFunction drawFunction, bool canRemove = true) {
		if (ent == NULL) { return; }

		const ImGuiTreeNodeFlags treeNodeFlags = 
		ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | 
		ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;

		if (ent->HasComponent<T>()) {
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
				drawFunction(ent->GetComponent<T>());

				ImGui::TreePop();
			}

			if (removeComponent) {
				ent->RemoveComponent<T>();
			}
		}
	}

	void Heirarchy::OnDraw(const Camera& camera, const ref<Scene>& scene) {
		ImGui::Begin("Heriarchy");

		for (const ref<Entity>& ent : scene->GetEntities()) {
			DrawEntityNode(ent);
		}

		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
			m_selectionContext = NULL;
		}

		ImGui::End();

		if (m_selectionContext != NULL) {
			std::string propertiesTitle = m_selectionContext->m_name + " Properties";
			ImGui::Begin((propertiesTitle + "###PROPERTIES").c_str());
		} else {
			ImGui::Begin("Properties###PROPERTIES");
		}

		DrawComponent<TransformComponent>("Transform", m_selectionContext, [](void* component){
			TransformComponent* tc = (TransformComponent*)component;

			DrawVec3Control("translation", tc->translation);
			DrawVec3Control("rotation", tc->rotation);
			DrawVec3Control("scale", tc->scale);
		}, true);

		ImGui::End();
	}
}