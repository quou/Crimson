#include <crimson.h>
#include <imgui.h>

#include "utils/tinyfiledialogs.h"
#include "panel.h"
#include "heirarchy.h"
#include "viewport.h"
#include "editor.h"

namespace Crimson {
	void Editor::Save() {
		if (m_currentSaveFile.empty()) {
			SaveAs();
			return;
		}

		SceneSerialiser s(m_scene);
		s.SerialiseScene(m_currentSaveFile.c_str());
	}

	void Editor::SaveAs() {
		std::string path = tinyfd_saveFileDialog("Save Scene As", (m_currentSavePath + m_currentSaveFile).c_str(), 0, NULL, "Scene Files");
		std::string file = path;
		file = file.substr(file.find_last_of("/") + 1);
		path.erase(path.find_last_of('/'));

		m_currentSaveFile = file;
		m_currentSavePath = path;

		AssetManager::Quit();
		AssetManager::Init(path.c_str());

		SceneSerialiser s(m_scene);
		s.SerialiseScene(file.c_str());
	}

	void Editor::Open() {
		std::string path = tinyfd_openFileDialog("Open Scene", m_currentSavePath.c_str(), 0, NULL, "Scene Files", false);
		std::string file = path;
		file = file.substr(file.find_last_of("/") + 1);
		path.erase(path.find_last_of('/'));

		m_currentSaveFile = file;
		m_currentSavePath = path;

		AssetManager::Quit();
		AssetManager::Init(path.c_str());

		m_scene = ref<Scene>(new Scene());
		SceneSerialiser s(m_scene);
		s.DeserialiseScene(file.c_str(), true);
	}

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

		// SceneSerialiser s(m_scene);
		// s.DeserialiseScene("test.crimson", true);
	}

	void Editor::OnUpdate(float delta) {
		if (m_isRunning) {
			m_scene->UpdateAndRefresh(delta);
		} else {
			m_scene->Refresh();
		}

		/* Must be called before AssetManager::HotReload, otherwise
		 * it won't detect any changes */
		if (m_scene->m_scriptManager->HotReload()) {
			m_scene->UpdateBehaviourInstances();
		}
		
		AssetManager::HotReload();

		Renderer::Clear(0.0f, 0.0f, 0.0f);

		/* Draw panels */
		ImGuiManager::BeginFrame();
		m_panelManager->Draw(m_camera, m_scene);

		/* Draw the main menu bar */
		ImGui::BeginMainMenuBar();
		if (ImGui::BeginMenu("file")) {
			if (ImGui::MenuItem("open")) {
				Open();
			}

			if (ImGui::MenuItem("save")) {
				Save();
			}

			if (ImGui::MenuItem("save as")) {
				SaveAs();
			}

			ImGui::EndMenu();
		}

		/* Draw the play/stop button */
		if (ImGui::Button(m_isRunning ? ICON_FK_STOP : ICON_FK_PLAY)) {
			if (m_isRunning) { /* Stop execution */
				m_scene = ref<Scene>(new Scene());
				SceneSerialiser ss(m_scene);

				ss.DeserialiseSceneFromMemory(m_currentSave, true);
			} else { /* Start execution */
				SceneSerialiser ss(m_scene);
				m_currentSave = ss.SerialiseScene();

				m_scene = ref<Scene>(new Scene());
				ss = SceneSerialiser(m_scene);
				ss.DeserialiseSceneFromMemory(m_currentSave);
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
