#pragma once

#include "panel.h"

namespace Crimson {
	class Console : public Panel {
	private:
	public:
		void OnDraw(const ref<Scene>& scene, float delta) override;
	};
}