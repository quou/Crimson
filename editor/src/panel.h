#pragma once

#include <crimson.h>

namespace Crimson {
	class Panel {
	protected:
		void* m_userData;

		friend class PanelManager;
	public:
		virtual void OnDraw(const Camera& camera, const ref<Scene>& scene) = 0;

		virtual ~Panel() {}
	};

	class PanelManager {
	private:
		std::vector<ref<Panel>> m_panels;

		void* m_userData;
	public:
		PanelManager(void* userData) : m_userData(userData) {}

		void AddPanel(const ref<Panel>& panel);

		void Draw(const Camera& camera, const ref<Scene>& scene);
	};
}