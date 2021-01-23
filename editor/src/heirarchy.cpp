#include <imgui.h>

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

	void Heirarchy::OnDraw(const Camera& camera, const ref<Scene>& scene) {
		ImGui::Begin("Heriarchy");

		for (const ref<Entity>& ent : scene->GetEntities()) {
			DrawEntityNode(ent);
		}

		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
			m_selectionContext = NULL;
		}

		ImGui::End();
	}
}