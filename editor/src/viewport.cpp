#include <imgui.h>

#include "viewport.h"
#include "editor.h"

namespace Crimson {
	Viewport::Viewport() {
		m_renderTarget = ref<RenderTarget>(new RenderTarget(vec2(800, 600)));
	}
	
	void Viewport::OnDraw(const ref<Scene>& scene, float delta) {
		Editor* editor = (Editor*)m_userData;

		ImGui::Begin("viewport");

		vec2 size = vec2(ImGui::GetWindowSize().x - 17, ImGui::GetWindowSize().y - 37);
		m_renderTarget->Resize(size);

		/* Update perspective */
		m_camera.Update(size, editor);

		if (ImGui::IsWindowHovered() && editor->m_window->m_eventSystem->MouseButtonPressed(CR_MOUSE_BUTTON_RIGHT)) {
			m_camera.Move(delta, editor);
		}

		m_renderTarget->Bind();
		scene->Draw(m_camera.GetCamera());
		m_renderTarget->Unbind();

		ImGui::Image((ImTextureID)(unsigned long)m_renderTarget->GetOutput(), ImVec2(size.x, size.y));

		ImGui::End();
	}
}