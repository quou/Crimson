#include <imgui.h>

#include "viewport.h"

namespace Crimson {
	Viewport::Viewport() {
		m_renderTarget = ref<RenderTarget>(new RenderTarget(vec2(800, 600)));
	}
	
	void Viewport::OnDraw(const Camera& camera, const ref<Scene>& scene) {
		ImGui::Begin("viewport");

		vec2 size = vec2(ImGui::GetWindowSize().x - 17, ImGui::GetWindowSize().y - 37);
		m_renderTarget->Resize(size);

		/* Update perspective */
		camera.projection = mat4::persp(70.0f, size.x/size.y, 0.1f, 20.0f);

		m_renderTarget->Bind();
		scene->Draw(camera);
		m_renderTarget->Unbind();

		ImGui::Image((ImTextureID)(unsigned long)m_renderTarget->GetOutput(), ImVec2(size.x, size.y));

		ImGui::End();
	}
}