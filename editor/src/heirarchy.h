#pragma once

#include "panel.h"

namespace Crimson {
	class Heirarchy : public Panel {
	private:
		Entity* m_selectionContext;
		
		void DrawEntityNode(const ref<Entity>& ent);
	public:
		void OnDraw(const Camera& camera, const ref<Scene>& scene) override;
	};
}