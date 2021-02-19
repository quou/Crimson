#pragma once

#include "panel.h"

namespace Crimson {
	class Hierarchy : public Panel {
	private:
		Entity* m_selectionContext;
		
		void DrawEntityNode(Entity* ent);
	public:
		void OnDraw(const ref<Scene>& scene, float delta) override;
	};
}