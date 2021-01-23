#pragma once

#include "panel.h"

namespace Crimson {
	class Heirarchy : public Panel {
	private:
	public:
		void OnDraw(const ref<Scene>& scene) override;
	};
}