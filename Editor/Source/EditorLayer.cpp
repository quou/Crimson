#include "EditorLayer.h"

#include <imgui.h>

EditorLayer::EditorLayer(Crimson::RenderTarget* renderTarget) : m_renderTarget(renderTarget) {}

void EditorLayer::OnInit() {
	ImGuiStyle& style = ImGui::GetStyle();
	style.Alpha = 1.0f;
	style.FrameRounding = 0.0f;
	style.WindowRounding = 0.0f;

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigWindowsResizeFromEdges = true;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void EditorLayer::OnUpdate(float delta) {
	ImGui::DockSpaceOverViewport();

	ImGui::Begin("Scene Viewport");

	m_renderTarget->Resize({ImGui::GetWindowSize().x - 15, ImGui::GetWindowSize().y - 35});

	ImGui::Image((ImTextureID)m_renderTarget->GetOutput(), ImVec2(ImGui::GetWindowSize().x - 15, ImGui::GetWindowSize().y - 35), ImVec2(0, 0), ImVec2(1, -1));

	ImGui::End();
}
