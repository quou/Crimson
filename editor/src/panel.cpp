#include "panel.h"

namespace Crimson {
	void PanelManager::AddPanel(const ref<Panel>& panel) {
		m_panels.push_back(panel);
	}

	void PanelManager::Draw(const Camera& camera, const ref<Scene>& scene) {
		for (auto& panel : m_panels) {
			panel->OnDraw(camera, scene);
		}
	}
}