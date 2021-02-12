#pragma once

#include "panel.h"
#include "scenecamera.h"

namespace Crimson {
	class Viewport : public Panel {
	private:
		ref<RenderTarget> m_renderTarget;
		ref<RenderTarget> m_gameRenderTarget;

		SceneCamera m_camera;

		int m_gizmoType = 0;
		float m_snapRotation = 45.0f;
		float m_snapTranslation = 0.5f;
	public:
		Viewport();

		void OnDraw(const ref<Scene>& scene, float delta) override;
	};
}