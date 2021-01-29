#pragma once

#include "panel.h"

namespace Crimson {
	class Console : public Panel {
	private:
	public:
		void OnDraw(const Camera& camera, const ref<Scene>& scene) override;
	};
}