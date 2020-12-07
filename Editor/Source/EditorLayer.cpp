#include "EditorLayer.h"

#include <filesystem>

#include <imgui.h>
#include <imgui_internal.h>

#include <Utils/tinyfiledialogs.h>

#include "Editor.h"

#include "FontAwesome.h"
#include "Fonts/Roboto-Regular.h"
#include "Fonts/FontAwesome4.h"

#include "UIUtils.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

EditorLayer::EditorLayer(SceneCamera* sceneCamera, Crimson::RenderTarget* sceneRenderTarget, Crimson::RenderTarget* gameRenderTarget, Crimson::Scene* scene)
 : m_camera(sceneCamera), m_sceneRenderTarget(sceneRenderTarget), m_gameRenderTarget(gameRenderTarget),
  	m_sceneHierarchyPanel(scene), m_assetManagerPanel(this) {}

static void ApplyDefaultTheme() {
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 0.95f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.12f, 0.12f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.53f, 0.53f, 0.53f, 0.46f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.85f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.22f, 0.22f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 0.53f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.48f, 0.48f, 0.48f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.79f, 0.79f, 0.79f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.48f, 0.47f, 0.47f, 0.91f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.55f, 0.55f, 0.62f);
	colors[ImGuiCol_Button] = ImVec4(0.50f, 0.50f, 0.50f, 0.63f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.67f, 0.67f, 0.68f, 0.63f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.26f, 0.26f, 0.26f, 0.63f);
	colors[ImGuiCol_Header] = ImVec4(0.54f, 0.54f, 0.54f, 0.58f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.64f, 0.65f, 0.65f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 0.25f, 0.25f, 0.80f);
	colors[ImGuiCol_Separator] = ImVec4(0.58f, 0.58f, 0.58f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.64f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.81f, 0.81f, 0.81f, 0.64f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.87f, 0.87f, 0.87f, 0.53f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.87f, 0.87f, 0.87f, 0.74f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.87f, 0.87f, 0.87f, 0.74f);
	colors[ImGuiCol_Tab] = ImVec4(0.01f, 0.01f, 0.01f, 0.86f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.02f, 0.02f, 0.02f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.38f, 0.48f, 0.60f, 1.00f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.68f, 0.68f, 0.68f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.77f, 0.33f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.87f, 0.55f, 0.08f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.47f, 0.60f, 0.76f, 0.47f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.58f, 0.58f, 0.58f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void EditorLayer::OnInit() {
	ImGuiStyle& style = ImGui::GetStyle();
	style.Alpha = 1.0f;
	style.FrameRounding = 0.0f;
	style.WindowRounding = 0.0f;
	style.WindowMinSize = ImVec2(300, 150);

	style.ChildRounding = 0.0f;
	style.FrameBorderSize = 1.0f;
	style.FrameRounding = 0.0f;
	style.GrabMinSize = 7.0f;
	style.PopupRounding = 0.0f;
	style.ScrollbarRounding = 12.0f;
	style.ScrollbarSize = 13.0f;
	style.TabBorderSize = 1.0f;
	style.TabRounding = 0.0f;
	style.WindowRounding = 0.0f;
	style.WindowMenuButtonPosition = ImGuiDir_None;

	ApplyDefaultTheme();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigWindowsResizeFromEdges = true;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.Fonts->AddFontFromFileTTF("Data/Fonts/Roboto-Regular.ttf", 14.0f);
	io.Fonts->AddFontFromMemoryCompressedTTF(Roboto_compressed_data, Roboto_compressed_size, 14.0f);

	ImFontConfig config;
	config.MergeMode = true;
	static const ImWchar icon_ranges[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };
	io.Fonts->AddFontFromMemoryCompressedTTF(FontAwesome4_compressed_data, FontAwesome4_compressed_size, 14.0f, &config, icon_ranges);


	auto editor = (Editor*)m_userData;

	// auto s = Crimson::SceneSerialiser(*editor->m_scene);
	// s.DeserialiseText(editor->m_scene->m_assetManager.LoadText("Data/Scenes/Test.cscn"));
	// m_currentSavePath = "Data/Scenes/Test.cscn";

	ImGui::SetWindowFocus("Scene Viewport");

	m_codeEditorPanel.Init();
}

void EditorLayer::OnExit() {
	auto editor = (Editor*)m_userData;

	auto shouldExit = tinyfd_messageBox("Warning", "You may have unsaved changes. Are you sure you want to quit?", "yesno", "warning", 0);

	if (!shouldExit) {
		editor->CancelExit();
	} else {
		editor->Exit();
	}
}

void EditorLayer::SaveAs() {
	auto editor = (Editor*)m_userData;

	const char* const acceptedExtensions[] = {"*.cscn", "*.scene", "*.crimson", "*.yaml"};
	const char* file = tinyfd_saveFileDialog("Save Scene", "Data/NewScene.cscn", 4, acceptedExtensions, "Crimson Scene Files");

	if (file) {
		m_currentSavePath = file;

		Crimson::SceneSerialiser sceneSerialiser(*editor->m_scene);
		sceneSerialiser.SerialiseText(file);
	}
}

void EditorLayer::ReloadScene() {
	auto editor = (Editor*)m_userData;

	Crimson::SceneSerialiser outSeriliser(*editor->m_scene);
	std::string file = outSeriliser.SerialiseString();

	editor->m_scene = std::make_shared<Crimson::Scene>(false);
	m_sceneHierarchyPanel.SetContext(&*editor->m_scene);
	m_sceneHierarchyPanel.SetSelectionContext(Crimson::Entity());

	editor->m_scene->m_assetManager.SetWorkingDir(m_workingDir);

	Crimson::SceneSerialiser inSerialiser(*editor->m_scene);
	inSerialiser.DeserialiseText(file);
}

void EditorLayer::SaveScene() {
	if (m_isRunning) {
		CR_LOG_WARNING("%s", "Saving in play mode may result in possible loss of data. Press CTRL+SHIFT+S to override")
		return;
	}

	auto editor = (Editor*)m_userData;

	if (!m_codeEditorPanel.IsFocused()) {
		if (!m_currentSavePath.empty()) {
			Crimson::SceneSerialiser sceneSerialiser(*editor->m_scene);
			sceneSerialiser.SerialiseText(m_workingDir + m_currentSavePath);
			CR_LOG("Scene saved to %s%s", m_workingDir.c_str(), m_currentSavePath.c_str());
		} else {
			SaveAs();
		}
	} else {
		m_codeEditorPanel.Save();
	}
}

void EditorLayer::NewScene() {
	auto editor = (Editor*)m_userData;

	const char* const acceptedExtensions[] = {"*.cscn", "*.scene", "*.crimson", "*.yaml"};

	const char* file = tinyfd_saveFileDialog("Create New Scene", "NewScene.cscn", 4, acceptedExtensions, "Crimson Scene Files");

	if (file) {
		m_currentSavePath = file;

		editor->m_scene = std::make_shared<Crimson::Scene>(false);
		m_sceneHierarchyPanel.SetContext(&*editor->m_scene);
		m_sceneHierarchyPanel.SetSelectionContext(Crimson::Entity());
		editor->m_scene->m_assetManager.SetWorkingDir(m_workingDir);

		auto light = editor->m_scene->CreateEntity("Main Light");
		light.GetComponent<Crimson::TransformComponent>().rotation = glm::radians(glm::vec3(10.0f, -20.0f, 25.0f));
		light.AddComponent<Crimson::AmbientLightComponent>(glm::vec3(1,1,1), 0.1f);
		light.AddComponent<Crimson::DirectionalLightComponent>(glm::vec3(1,1,1), 1.0f);

		auto cam = editor->m_scene->CreateEntity("Main Camera");
		cam.AddComponent<Crimson::CameraComponent>(std::pair<int, int>{1366, 768}, 45.0f);

		auto cube = editor->m_scene->CreateEntity("Cube");
		cube.AddComponent<Crimson::MeshFilterComponent>("Cube");
		cube.AddComponent<Crimson::MaterialComponent>("Default");

		Crimson::SceneSerialiser sceneSerialiser(*editor->m_scene);
		sceneSerialiser.SerialiseText(file);
	}
}

void EditorLayer::OpenProject() {
	auto editor = (Editor*)m_userData;

	auto folder = tinyfd_selectFolderDialog("Open Project Folder", "");
	if (folder) {
		m_workingDir = std::string(folder) + "/";
		editor->m_scene->m_assetManager.SetWorkingDir(m_workingDir);
		m_assetManagerPanel.Refresh();
		Crimson::Input::LoadConfig(editor->m_scene->m_assetManager.LoadText("Data/InputConfig.conf").c_str());
	}
}

void EditorLayer::RunScene() {
	ImGui::StyleColorsDark();

	ImGui::SetWindowFocus("Game Viewport");

	auto editor = (Editor*)m_userData;

	Crimson::SceneSerialiser sceneSerialiser(*editor->m_scene);
	m_currentSerialiseString = sceneSerialiser.SerialiseString();

	editor->m_scene = std::make_shared<Crimson::Scene>(false);
	m_sceneHierarchyPanel.SetContext(&*editor->m_scene);
	m_sceneHierarchyPanel.SetSelectionContext(Crimson::Entity());

	editor->m_scene->m_assetManager.SetWorkingDir(m_workingDir);

	Crimson::SceneSerialiser inSerialiser(*editor->m_scene);
	inSerialiser.DeserialiseText(m_currentSerialiseString);

	editor->m_scene->Init();

	m_isRunning = true;
}

void EditorLayer::StopRunning() {
	ApplyDefaultTheme();

	ImGui::SetWindowFocus("Scene Viewport");

	auto editor = (Editor*)m_userData;

	editor->m_scene = std::make_shared<Crimson::Scene>(false);
	m_sceneHierarchyPanel.SetContext(&*editor->m_scene);
	m_sceneHierarchyPanel.SetSelectionContext(Crimson::Entity());

	editor->m_scene->m_assetManager.SetWorkingDir(m_workingDir);

	Crimson::SceneSerialiser inSerialiser(*editor->m_scene);
	inSerialiser.DeserialiseText(m_currentSerialiseString);

	m_isRunning = false;
}

void EditorLayer::OnUpdate(float delta) {
	auto editor = (Editor*)m_userData;

	if (!m_workingDir.empty()) {
		std::replace(m_workingDir.begin(), m_workingDir.end(), '\\', '/');
	}

	editor->m_scene->m_isUpdating = m_isRunning;
	if (m_isRunning) {
		editor->m_scene->Update(delta);
	}

	ImGui::DockSpaceOverViewport();

	m_consolePanel.Render();
	m_codeEditorPanel.Render(delta);
	m_assetManagerPanel.Render((Editor*)m_userData, m_sceneHierarchyPanel, m_codeEditorPanel);
	m_sceneHierarchyPanel.Render(m_assetManagerPanel);

	static bool showExportPopup = false;

	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File")) {
		if (ImGui::MenuItem("Open Project Folder", "CTRL+O")) {
			OpenProject();
		}

		if (ImGui::MenuItem("New Project")) {
			const char* folder = tinyfd_selectFolderDialog("Select Folder for New Project", "");

			if (folder) {
				std::string f = folder;

				std::replace(f.begin(), f.end(), '\\', '/');

				std::filesystem::create_directories(f + "/Data/");
				std::filesystem::create_directories(f + "/Data/Scenes/");

				// Create project config
				std::ofstream configF(f + "/" + "Data/ProjectConfig.conf");
				if (configF.good()) {
					configF << "project = {\n";
					configF << "\t" << "startup = \"" << "Data/Scenes/SampleScene.cscn" << "\"\n";
					configF << "}";
				}
				configF.close();

				Crimson::Input::SaveConfig(std::string(f + "/" + "Data/InputConfig.conf").c_str());


				// Create Sample Scene
				editor->m_scene = std::make_shared<Crimson::Scene>(false);
				m_sceneHierarchyPanel.SetContext(&*editor->m_scene);
				m_sceneHierarchyPanel.SetSelectionContext(Crimson::Entity());

				auto light = editor->m_scene->CreateEntity("Main Light");
				light.GetComponent<Crimson::TransformComponent>().rotation = glm::radians(glm::vec3(10.0f, -20.0f, 25.0f));
				light.AddComponent<Crimson::AmbientLightComponent>(glm::vec3(1,1,1), 0.1f);
				light.AddComponent<Crimson::DirectionalLightComponent>(glm::vec3(1,1,1), 1.0f);

				auto cam = editor->m_scene->CreateEntity("Main Camera");
				cam.AddComponent<Crimson::CameraComponent>(std::pair<int, int>{1366, 768}, 45.0f);

				auto cube = editor->m_scene->CreateEntity("Cube");
				cube.AddComponent<Crimson::MeshFilterComponent>("Cube");
				cube.AddComponent<Crimson::MaterialComponent>("Default");

				Crimson::SceneSerialiser sceneSerialiser(*editor->m_scene);
				sceneSerialiser.SerialiseText(f + "/Data/Scenes/SampleScene.cscn");
				m_currentSavePath = f + "/Data/Scenes/SampleScene.cscn";

				m_workingDir = std::string(folder) + "/";
				editor->m_scene->m_assetManager.SetWorkingDir(m_workingDir);
				m_assetManagerPanel.Refresh();
				Crimson::Input::LoadConfig(editor->m_scene->m_assetManager.LoadText("Data/InputConfig.conf").c_str());
			}
		}

		ImGui::Separator();

		if (ImGui::MenuItem("New Scene", "CTRL+N")) {
			NewScene();
		}

		if (ImGui::MenuItem("Save", "CTRL+S")) {
			SaveScene();
		}

		if (ImGui::MenuItem("Save As...", "CTRL+SHIFT+S")) {
			SaveAs();
		}

		ImGui::Separator();


		if (ImGui::MenuItem("Export Runtime")) {
			if (!m_workingDir.empty()) {
				showExportPopup = true;
			} else {
				CR_LOG_ERROR("%s", "Cannot export without a project folder being open");
			}
		}

		ImGui::Separator();
		if (ImGui::MenuItem("Quit")) {
			editor->Exit();
		}

		ImGui::EndMenu();
	}

	if (showExportPopup) {
		ImGui::OpenPopup("Export Runtime");
		showExportPopup = false;
	}

	if (ImGui::BeginPopupModal("Export Runtime")) {
		std::vector<std::string> scenePaths;

		try {
			std::string path = m_workingDir + "Data/";
			for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
				if (entry.path().extension().string() == ".cscn") {
					scenePaths.push_back(entry.path().string());
				}
			}
		} catch (const std::exception& e) {
			CR_LOG_ERROR("%s", e.what());
			ImGui::CloseCurrentPopup();
		}

		std::string startupScenePath = DrawComboBox("Startup Scene", scenePaths);
		std::replace(startupScenePath.begin(), startupScenePath.end(), '\\', '/');

		CR_LOG("%s", startupScenePath.c_str());
		CR_LOG("%s", m_workingDir.c_str());

		// Erase the working directory
		size_t pos = startupScenePath.find(m_workingDir);
		if (pos != std::string::npos) {
			startupScenePath.erase(pos, m_workingDir.length());
		}
		ImGui::Text("%s", startupScenePath.c_str());

		if (ImGui::Button("Export")) {

			const char* folder = tinyfd_selectFolderDialog("Export Runtime", "");

			if (folder) {
				Crimson::Input::SaveConfig(std::string(m_workingDir + "Data/InputConfig.conf").c_str());

				std::ofstream configF(m_workingDir + "Data/ProjectConfig.conf");
				if (configF.good()) {
					configF << "project = {\n";
					configF << "\t" << "startup = \"" << startupScenePath << "\"\n";
					configF << "}";
				}
				configF.close();

				#ifdef _WIN32
					remove(std::string(std::string(folder) + "/" + "Game.exe").c_str());
					Crimson::CopyFile("Game.exe", std::string(std::string(folder) + "/" + "Game.exe"));
				#else
					remove(std::string(std::string(folder) + "/" + "Game").c_str());
					Crimson::CopyFile("Game", std::string(std::string(folder) + "/" + "Game"));
				#endif

				remove(std::string(std::string(folder) + "/" + "Data.pck").c_str());
				Crimson::CompressFolder("Data", std::string(folder) + "/" + "Data.pck", m_workingDir);
			}


			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}


	if (ImGui::BeginMenu("Scene")) {
		if (ImGui::MenuItem("Reload", "CTRL+SHIFT+R")) {
			ReloadScene();
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Run")) {
			RunScene();
		}

		if (ImGui::MenuItem("Stop")) {
			StopRunning();
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Tools")) {
		if (ImGui::MenuItem("Select", "CTRL+Q")) {
			m_gizmoType = -1;
		}

		if (ImGui::MenuItem("Translate", "CTRL+W")) {
			m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
		}

		if (ImGui::MenuItem("Rotate", "CTRL+E")) {
			m_gizmoType = ImGuizmo::OPERATION::ROTATE;
		}

		if (ImGui::MenuItem("Scale", "CTRL+R")) {
			m_gizmoType = ImGuizmo::OPERATION::SCALE;
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("View")) {
		ImGui::MenuItem("Profiler", "", &m_showProfiler);

		ImGui::EndMenu();
	}

	if (m_isRunning) {
		if (ImGui::Button(ICON_FK_STOP)) {
			StopRunning();
		}
	} else {
		if (ImGui::Button(ICON_FK_PLAY)) {
			RunScene();
		}
	}

	ImGui::EndMainMenuBar();

	// CTRL+O
	if (ImGui::IsKeyDown(CR_KEY_LEFT_CONTROL) && ImGui::IsKeyPressed(CR_KEY_O)) {
		OpenProject();
	}

	// CTRL+N
	if (ImGui::IsKeyDown(CR_KEY_LEFT_CONTROL) && ImGui::IsKeyPressed(CR_KEY_N)) {
		NewScene();
	}

	// CTRL+S
	if (ImGui::IsKeyDown(CR_KEY_LEFT_CONTROL) && ImGui::IsKeyPressed(CR_KEY_S)) {
		SaveScene();
	}

	// CTRL+SHIFT+S
	if (ImGui::IsKeyDown(CR_KEY_LEFT_CONTROL) && ImGui::IsKeyDown(CR_KEY_LEFT_SHIFT) && ImGui::IsKeyPressed(CR_KEY_S)) {
		SaveAs();
	}

	// CTRL+SHIFT+R
	if (ImGui::IsKeyDown(CR_KEY_LEFT_CONTROL)  && ImGui::IsKeyDown(CR_KEY_LEFT_SHIFT) && ImGui::IsKeyPressed(CR_KEY_R)) {
		ReloadScene();
	}

	// Gizmo shortcuts
	if (!ImGuizmo::IsUsing()) {
		// CTRL+Q
		if (ImGui::IsKeyDown(CR_KEY_LEFT_CONTROL) && ImGui::IsKeyPressed(CR_KEY_Q)) {
			m_gizmoType = -1;
		}
		// CTRL+W
		if (ImGui::IsKeyDown(CR_KEY_LEFT_CONTROL) && ImGui::IsKeyPressed(CR_KEY_W)) {
			m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
		}
		// CTRL+E
		if (ImGui::IsKeyDown(CR_KEY_LEFT_CONTROL) && ImGui::IsKeyPressed(CR_KEY_E)) {
			m_gizmoType = ImGuizmo::OPERATION::ROTATE;
		}
		// CTRL+Q
		if (ImGui::IsKeyDown(CR_KEY_LEFT_CONTROL) && ImGui::IsKeyPressed(CR_KEY_R)) {
			m_gizmoType = ImGuizmo::OPERATION::SCALE;
		}
	}

	ImGui::Begin("Game Viewport");
	if (!ImGui::IsWindowCollapsed()) {
		m_gameRenderTarget->Resize({ImGui::GetWindowSize().x - 15, ImGui::GetWindowSize().y - 37});

		ImGui::Image((ImTextureID)m_gameRenderTarget->GetOutput(), ImVec2(ImGui::GetWindowSize().x - 15, ImGui::GetWindowSize().y - 37), ImVec2(0, 0), ImVec2(1, -1));
	}
	ImGui::End();

	ImGui::Begin("Scene Viewport");

	if (!ImGui::IsWindowCollapsed()) {
		m_sceneRenderTarget->Resize({ImGui::GetWindowSize().x - 15, ImGui::GetWindowSize().y - 37});

		ImGui::Image((ImTextureID)m_sceneRenderTarget->GetOutput(), ImVec2(ImGui::GetWindowSize().x - 15, ImGui::GetWindowSize().y - 37), ImVec2(0, 0), ImVec2(1, -1));

		if (ImGui::IsItemHovered()) {
			if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
				m_camera->Update(delta);
			}
			m_camera->UpdateScroll(delta);
		}

		// Gizmos
		auto selectedEntity = m_sceneHierarchyPanel.m_selectedEntity;
		if (selectedEntity && m_gizmoType > -1) {
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

			// Camera
			auto camera = editor->m_camera.GetCamera();
			glm::mat4 view = camera->GetView();
			glm::mat4 projection = camera->projection;

			// Entity transform
			auto& tc = selectedEntity.GetComponent<Crimson::TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = ImGui::IsKeyDown(CR_KEY_LEFT_CONTROL);
			float snapValue = m_snapTranslation;
			if (m_gizmoType == ImGuizmo::OPERATION::ROTATE) {
				snapValue = m_snapRotation;
			}

			float snapValues[3] = {snapValue, snapValue, snapValue};

			ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection),
										(ImGuizmo::OPERATION)m_gizmoType, ImGuizmo::MODE::LOCAL, glm::value_ptr(transform),
										NULL, snap ? snapValues : NULL);

			if (ImGuizmo::IsUsing()) {
				glm::vec3 skew; glm::vec4 perspective; // Needed for glm::decompose, not actually in use
				glm::decompose(transform, tc.scale, tc.rotation, tc.position, skew, perspective);
			}
		}
	}
	ImGui::End();

	ImGui::Begin("Scene Config");

	DrawTextControl("Name", editor->m_scene->GetConfig().name);
	DrawColorControl("Clear Color", editor->m_scene->GetConfig().clearColor);
	DrawBoolControl("Use Skybox", &editor->m_scene->GetConfig().useSkybox);
	if (editor->m_scene->GetConfig().useSkybox) {
		ImGui::Separator();
		ImGui::Text("Skybox");
		DrawTextLabel("Positive X", editor->m_scene->GetConfig().skyboxPosX);
		if (ImGui::BeginDragDropTarget()) {
			if (ImGuiPayload const* payload = ImGui::AcceptDragDropPayload("Image")) {
				std::string toSet = static_cast<const char*>(payload->Data);
				editor->m_scene->GetConfig().skyboxPosX = toSet;
			}
			ImGui::EndDragDropTarget();
		}

		DrawTextLabel("Negative X", editor->m_scene->GetConfig().skyboxNegX);
		if (ImGui::BeginDragDropTarget()) {
			if (ImGuiPayload const* payload = ImGui::AcceptDragDropPayload("Image")) {
				std::string toSet = static_cast<const char*>(payload->Data);
				editor->m_scene->GetConfig().skyboxNegX = toSet;
			}
			ImGui::EndDragDropTarget();
		}

		DrawTextLabel("Positive Y", editor->m_scene->GetConfig().skyboxPosY);
		if (ImGui::BeginDragDropTarget()) {
			if (ImGuiPayload const* payload = ImGui::AcceptDragDropPayload("Image")) {
				std::string toSet = static_cast<const char*>(payload->Data);
				editor->m_scene->GetConfig().skyboxPosY = toSet;
			}
			ImGui::EndDragDropTarget();
		}

		DrawTextLabel("Negative Y", editor->m_scene->GetConfig().skyboxNegY);
		if (ImGui::BeginDragDropTarget()) {
			if (ImGuiPayload const* payload = ImGui::AcceptDragDropPayload("Image")) {
				std::string toSet = static_cast<const char*>(payload->Data);
				editor->m_scene->GetConfig().skyboxNegY = toSet;
			}
			ImGui::EndDragDropTarget();
		}

		DrawTextLabel("Positive Z", editor->m_scene->GetConfig().skyboxPosZ);
		if (ImGui::BeginDragDropTarget()) {
			if (ImGuiPayload const* payload = ImGui::AcceptDragDropPayload("Image")) {
				std::string toSet = static_cast<const char*>(payload->Data);
				editor->m_scene->GetConfig().skyboxPosZ = toSet;
			}
			ImGui::EndDragDropTarget();
		}

		DrawTextLabel("Negative Z", editor->m_scene->GetConfig().skyboxNegZ);
		if (ImGui::BeginDragDropTarget()) {
			if (ImGuiPayload const* payload = ImGui::AcceptDragDropPayload("Image")) {
				std::string toSet = static_cast<const char*>(payload->Data);
				editor->m_scene->GetConfig().skyboxNegZ = toSet;
			}
			ImGui::EndDragDropTarget();
		}
	}

	ImGui::End();

	if (m_showProfiler) {
		ImGui::Begin("Profiler", &m_showProfiler);

		{
			static float values[90] = {};
         static int values_offset = 0;
         static double refresh_time = 0.0;
         while (refresh_time < ImGui::GetTime())
         {
             static float phase = 0.0f;
             values[values_offset] = delta;
             values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
             phase += 0.10f * values_offset;
             refresh_time += 1.0f / 60.0f;
         }

			DrawLinePlot("Frame Time", values, IM_ARRAYSIZE(values), values_offset, 0.0f, 0.1f, 150.0f);
		}

		{
			ImGui::Separator(); ImGui::Text("Renderer");
			static float values[90] = {};
         static int values_offset = 0;
         static double refresh_time = 0.0;
         while (refresh_time < ImGui::GetTime())
         {
             static float phase = 0.0f;
             values[values_offset] = Crimson::Renderer::GetUpdateTime();
             values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
             phase += 0.10f * values_offset;
             refresh_time += 1.0f / 60.0f;
         }

			DrawLinePlot("Update Time", values, IM_ARRAYSIZE(values), values_offset, 0.0f, 0.1f, 150.0f);

			DrawTextLabel("Draw Calls", std::to_string(Crimson::Renderer::GetDrawCallsCount()/2));
		}

		{
			ImGui::Separator(); ImGui::Text("Script Engine");
			static float values[90] = {};
         static int values_offset = 0;
         static double refresh_time = 0.0;
         while (refresh_time < ImGui::GetTime())
         {
             static float phase = 0.0f;
             values[values_offset] = editor->m_scene->GetScriptEngineUpdateTime();
             values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
             phase += 0.10f * values_offset;
             refresh_time += 1.0f / 60.0f;
         }

			DrawLinePlot("Update Time", values, IM_ARRAYSIZE(values), values_offset, 0.0f, 0.1f, 150.0f);
		}

		{
			static float values[90] = {};
         static int values_offset = 0;
         static double refresh_time = 0.0;
         while (refresh_time < ImGui::GetTime())
         {
             static float phase = 0.0f;
             values[values_offset] = editor->m_scene->GetScriptEngineMemoryUsage();
             values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
             phase += 0.10f * values_offset;
             refresh_time += 1.0f / 60.0f;
         }

			DrawLinePlot("Memory Usage (KB)", values, IM_ARRAYSIZE(values), values_offset, 0.0f, 1000.0f, 150.0f);
		}

		{
			ImGui::Separator(); ImGui::Text("Physics Engine");
			static float values[90] = {};
         static int values_offset = 0;
         static double refresh_time = 0.0;
         while (refresh_time < ImGui::GetTime())
         {
             static float phase = 0.0f;
             values[values_offset] = editor->m_scene->GetPhysicsEngineUpdateTime();
             values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
             phase += 0.10f * values_offset;
             refresh_time += 1.0f / 60.0f;
         }

			DrawLinePlot("Update Time", values, IM_ARRAYSIZE(values), values_offset, 0.0f, 0.1f, 150.0f);
		}

		ImGui::End();
	}
}
