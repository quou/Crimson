#pragma once

#include "panel.h"

namespace Crimson {
	class Viewport : public Panel {
	private:
		ref<RenderTarget> m_renderTarget;
	public:
		Viewport();

		void OnDraw(const Camera& camera, const ref<Scene>& scene) override;
	};
}