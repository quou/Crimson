#include <imgui.h>
#include "imgui/ImGuizmo.h"

#include "viewport.h"
#include "editor.h"

namespace Crimson {
	Viewport::Viewport() {
		m_renderTarget = ref<RenderTarget>(new RenderTarget(vec2(800, 600)));
		m_gameRenderTarget = ref<RenderTarget>(new RenderTarget(vec2(800, 600)));
	}
	
	void Viewport::OnDraw(const ref<Scene>& scene, float delta) {
		Editor* editor = (Editor*)m_userData;

		ImGui::Begin("viewport");
		{
			vec2 size = vec2(ImGui::GetWindowSize().x - 17, ImGui::GetWindowSize().y - 37);
			m_renderTarget->Resize(size);

			/* Update perspective */
			m_camera.Update(size, editor);

			if (ImGui::IsWindowHovered() && EventSystem::MouseButtonPressed(CR_MOUSE_BUTTON_RIGHT)) {
				m_camera.Move(delta, editor);
			}

			m_renderTarget->Bind();
			scene->Draw(m_camera.GetCamera());
			m_renderTarget->Unbind();

			ImGui::Image((ImTextureID)(unsigned long)m_renderTarget->GetOutput(), ImVec2(size.x, size.y));
		}
		ImGui::End();

		ImGui::Begin("game");

		Camera* camera = scene->GetMainCamera();

		if (camera) {
			vec2 size = vec2(ImGui::GetWindowSize().x - 17, ImGui::GetWindowSize().y - 37);
			m_gameRenderTarget->Resize(size);

			/* Update perspective */
			camera->projection = mat4::persp(45.0f, size.x/size.y, 0.1f, 1000.0f);

			m_gameRenderTarget->Bind();
			scene->Draw(*camera);
			m_gameRenderTarget->Unbind();

			ImGui::Image((ImTextureID)(unsigned long)m_gameRenderTarget->GetOutput(), ImVec2(size.x, size.y));
		} else {
			ImGui::Text("no cameras rendering");
		}

		ImGui::End();
	}
}