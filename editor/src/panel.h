#pragma once

#include <crimson.h>

namespace Crimson {
	class Panel {
	public:
		virtual void OnDraw(const ref<Scene>& scene) = 0;
	};

	class PanelManager {
	private:
		std::vector<ref<Panel>> m_panels;
	public:
		void AddPanel(const ref<Panel>& panel);

		void Draw(const ref<Scene>& scene);
	};
}