#pragma once

#include <crimson.h>

namespace Crimson {
	class Panel {
	public:
		virtual void OnDraw(const Camera& camera, const ref<Scene>& scene) = 0;

		virtual ~Panel() {}
	};

	class PanelManager {
	private:
		std::vector<ref<Panel>> m_panels;
	public:
		void AddPanel(const ref<Panel>& panel);

		void Draw(const Camera& camera, const ref<Scene>& scene);
	};
}