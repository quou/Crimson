#include <crimson.h>
#include <imgui.h>

#include "panel.h"
#include "heirarchy.h"
#include "viewport.h"

#include "editor.h"

namespace Crimson {
	void Editor::OnInit() {
		ImGuiManager::Init(m_window);

		/* Create panels */
		m_panelManager = ref<PanelManager>(new PanelManager(this));
		m_panelManager->AddPanel(ref<Panel>(new Heirarchy()));
		m_panelManager->AddPanel(ref<Panel>(new Viewport()));

		/* Create the scene */
		m_scene = ref<Scene>(new Scene());

		/* Create the camera */
		m_camera = Camera(m_window->GetWidth(), m_window->GetHeight(), 70.0f, 0.1f, 100.0f);
		m_camera.position = vec3(0.0f, 0.5f, 5.0f);

		SceneSerialiser s(m_scene);
		s.DeserialiseScene("test.crimson");
	}

	void Editor::OnUpdate(float delta) {
		if (m_isRunning) {
			m_scene->UpdateAndRefresh(delta);
		} else {
			m_scene->Refresh();
		}

		/* Must be called before AssetManager::HotReload, otherwise
		 * it won't detect any changes */
		m_scene->m_scriptManager->HotReload();
		
		AssetManager::HotReload();

		Renderer::Clear(0.0f, 0.0f, 0.0f);

		/* Draw panels */
		ImGuiManager::BeginFrame();
		m_panelManager->Draw(m_camera, m_scene);
		ImGui::BeginMainMenuBar();
		if (ImGui::BeginMenu("file")) {
			if (ImGui::MenuItem("save")) {
				SceneSerialiser s(m_scene);
				s.SerialiseScene("test.crimson");
			}

			ImGui::EndMenu();
		}

		if (ImGui::Button(m_isRunning ? ICON_FK_STOP : ICON_FK_PLAY)) {
			if (m_isRunning) {
				m_scene = ref<Scene>(new Scene());
				SceneSerialiser ss(m_scene);

				ss.DeserialiseSceneFromMemory(m_currentSave);
			} else {
				SceneSerialiser ss(m_scene);
				m_currentSave = ss.SerialiseScene();
			}

			m_isRunning = !m_isRunning;
		}

		ImGui::EndMainMenuBar(); 
		ImGuiManager::EndFrame();
	}

	void Editor::OnExit() {
		ImGuiManager::Quit();
	}
}

int main() {
	Crimson::Editor app;
	app.Run("Crimson Editor", 1366, 768);
}
