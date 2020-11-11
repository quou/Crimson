#include "EditorLayer.h"

#include <imgui.h>

#include <Utils/tinyfiledialogs.h>

EditorLayer::EditorLayer(SceneCamera* sceneCamera, Crimson::RenderTarget* renderTarget, Crimson::Scene* scene)
 : m_camera(sceneCamera), m_renderTarget(renderTarget),
  	m_sceneHierarchyPanel(scene) {}

void EditorLayer::OnInit() {
	ImGuiStyle& style = ImGui::GetStyle();
	style.Alpha = 1.0f;
	style.FrameRounding = 0.0f;
	style.WindowRounding = 0.0f;
	style.WindowMinSize = ImVec2(300, 350);

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


	ImGuiIO& io = ImGui::GetIO();
	io.ConfigWindowsResizeFromEdges = true;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.Fonts->AddFontFromFileTTF("Data/Fonts/Roboto-Regular.ttf", 14.0f);
}

static void ExportProgressCallback(int progress) {

}

void EditorLayer::OnUpdate(float delta) {
	ImGui::DockSpaceOverViewport();

	m_sceneHierarchyPanel.Render();
	m_assetManagerPanel.Render((Editor*)m_userData, m_sceneHierarchyPanel);

	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File")) {
		if (ImGui::MenuItem("Open...")) {
			const char* const acceptedExtensions[] = {"*.cscn"};

			tinyfd_openFileDialog("Open Scene", "", 0, acceptedExtensions, "Crimson Scene files", 0);
		}

		if (ImGui::MenuItem("Save")) {

		}

		if (ImGui::MenuItem("Save As...")) {

		}

		ImGui::Separator();

		if (ImGui::MenuItem("Export Runtime")) {
			const char* folder = tinyfd_selectFolderDialog("Export Runtime", "");

			if (folder) {
				#ifdef _WIN32
					remove(std::string(std::string(folder) + "/" + "Game.exe").c_str());
					Crimson::CopyFile("Game.exe", std::string(std::string(folder) + "/" + "Game.exe"));
				#else
					remove(std::string(std::string(folder) + "/" + "Game").c_str());
					Crimson::CopyFile("Game", std::string(std::string(folder) + "/" + "Game"));
				#endif

				remove(std::string(std::string(folder) + "/" + "Data.pck").c_str());
				Crimson::CompressFolder("Data", std::string(std::string(folder) + "/" + "Data.pck").c_str());
			}
		}

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	ImGui::Begin("Scene Viewport");

	if (!ImGui::IsWindowCollapsed()) {
		m_renderTarget->Resize({ImGui::GetWindowSize().x - 15, ImGui::GetWindowSize().y - 37});

		ImGui::Image((ImTextureID)m_renderTarget->GetOutput(), ImVec2(ImGui::GetWindowSize().x - 15, ImGui::GetWindowSize().y - 37), ImVec2(0, 0), ImVec2(1, -1));

		if (ImGui::IsItemHovered()) {
			if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
				m_camera->Update(delta);
			}
			m_camera->UpdateScroll(delta);
		}
	}
	ImGui::End();
}
