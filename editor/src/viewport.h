#pragma once

#include "panel.h"
#include "scenecamera.h"

namespace Crimson {
	class Viewport : public Panel {
	private:
		ref<RenderTarget> m_renderTarget;

		SceneCamera m_camera;
	public:
		Viewport();

		void OnDraw(const ref<Scene>& scene, float delta) override;
	};
}