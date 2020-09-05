#include "GUI.h"

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Utils/ImGuizmo.h>

#include <filesystem>
#include <algorithm>

void GUI::DrawHierarchy(ECS& ecs, Crimson::SceneManager& sceneManager) {
   ImGui::Begin("Hierarchy", &m_hierarchyOpen);
   for (EntityHandle ent : sceneManager.GetEntities()) {
      ImGui::Text("%s", ecs.GetComponent<Crimson::Transform>(ent)->name.c_str());
   }
   ImGui::End();
}

void GUI::DrawMainMenuBar() {
   if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("File")) {
         ImGui::End();
      }

      if (ImGui::BeginMenu("Windows")) {
         ImGui::MenuItem("Hierarchy", NULL, &m_hierarchyOpen);
         ImGui::End();
      }

      ImGui::EndMainMenuBar();
   }
}

GUI::GUI(SDL_Window* window, const SDL_GLContext glContext) {
   m_workingDir = std::filesystem::current_path().string() + "/";
   std::replace(m_workingDir.begin(), m_workingDir.end(), '\\', '/');
   Init(window, glContext);
}

void GUI::Update(const SDL_Event& event) {
   ImGui_ImplSDL2_ProcessEvent(&event);
}

void GUI::Init(SDL_Window* window, const SDL_GLContext glContext) {
   m_workingDir = std::filesystem::current_path().string() + "/";
   std::replace(m_workingDir.begin(), m_workingDir.end(), '\\', '/');
   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGuiIO& io = ImGui::GetIO(); (void)io;
   io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
   io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

   ImGuiStyle& style = ImGui::GetStyle();
   if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
   {
     style.WindowRounding = 0.0f;
     style.Colors[ImGuiCol_WindowBg].w = 1.0f;
   }

   ImGui_ImplSDL2_InitForOpenGL(window, glContext);
   ImGui_ImplOpenGL3_Init("#version 330 core");
}

void GUI::Render(SDL_Window* window, ECS& ecs, Crimson::SceneManager& sceneManager, Crimson::Camera& camera) {
   ImGui_ImplOpenGL3_NewFrame();
   ImGui_ImplSDL2_NewFrame(window);
   ImGui::NewFrame();
   ImGuizmo::BeginFrame();

   ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_PassthruCentralNode);

   DrawMainMenuBar();

   if (m_hierarchyOpen) {DrawHierarchy(ecs, sceneManager);}

   ImGui::Render();
   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
   ImGuiIO& io = ImGui::GetIO(); (void)io;
   int x, y, w, h;
   if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
      SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
   }
}

GUI::~GUI() {
   ImGui_ImplOpenGL3_Shutdown();
   ImGui_ImplSDL2_Shutdown();
   ImGui::DestroyContext();
}
