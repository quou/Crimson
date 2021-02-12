#pragma once

#include "panel.h"

namespace Crimson {
	class Console : public Panel {
	private:
		unsigned int m_lastSize = 0;
	public:
		void OnDraw(const ref<Scene>& scene, float delta) override;
	};
}