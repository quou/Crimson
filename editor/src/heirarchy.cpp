#include <imgui.h>

#include "heirarchy.h"

namespace Crimson {
	void Heirarchy::OnDraw(const ref<Scene>& scene) {
		ImGui::Begin("Heriarchy");

		for (const ref<Entity>& ent : scene->GetEntities()) {
			ImGui::Text("%s", ent->m_name.c_str());
		}

		ImGui::End();
	}
}