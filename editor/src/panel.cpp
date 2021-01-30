#include "panel.h"

namespace Crimson {
	void PanelManager::AddPanel(const ref<Panel>& panel) {
		panel->m_userData = m_userData;
		m_panels.push_back(panel);
	}

	void PanelManager::Draw(const ref<Scene>& scene, float delta) {
		for (auto& panel : m_panels) {
			panel->OnDraw(scene, delta);
		}
	}
}