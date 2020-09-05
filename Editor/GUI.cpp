#include "GUI.h"

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <filesystem>
#include <string>
#include <algorithm>

void GUI::DrawHierarchy(ECS& ecs, Crimson::SceneManager& sceneManager) {
   ImGui::Begin("Hierarchy", &m_hierarchyOpen);

   for (EntityHandle ent : sceneManager.GetEntities()) {
      if (!ecs.GetComponent<Crimson::Transform>(ent)->parent) {
         DrawEntityHierarchy(ecs, ent);
      }
   }
   ImGui::End();
}

void GUI::DrawInspector(ECS& ecs, Crimson::SceneManager& sceneManager) {
   ImGui::Begin("Inspector", &m_inspectorOpen);
   if (m_selectedEntity && ecs.HasComponent<Crimson::Transform>(m_selectedEntity)) {
      ImGui::Text("Selected Entity: %s", ecs.GetComponent<Crimson::Transform>(m_selectedEntity)->name.c_str());

      if (ImGui::RadioButton("Translate", m_currentGizmoOperation == ImGuizmo::TRANSLATE))
         m_currentGizmoOperation = ImGuizmo::TRANSLATE;
      ImGui::SameLine();
      if (ImGui::RadioButton("Rotate", m_currentGizmoOperation == ImGuizmo::ROTATE))
         m_currentGizmoOperation = ImGuizmo::ROTATE;
      ImGui::SameLine();
      if (ImGui::RadioButton("Scale", m_currentGizmoOperation == ImGuizmo::SCALE))
         m_currentGizmoOperation = ImGuizmo::SCALE;

      if (ImGui::CollapsingHeader("Transform")) {
         Crimson::Transform* t = ecs.GetComponent<Crimson::Transform>(m_selectedEntity);
         float newpos[] = {t->position.x, t->position.y, t->position.z};
         float newrot[] = {t->rotation.x, t->rotation.y, t->rotation.z};
         float newscale[] = {t->scale.x, t->scale.y, t->scale.z};

         ImGui::InputFloat3("Position", newpos);
         ImGui::InputFloat3("Rotation", newrot);
         ImGui::InputFloat3("Scale", newscale);

         t->position = glm::vec3(newpos[0], newpos[1], newpos[2]);
         t->rotation = glm::vec3(newrot[0], newrot[1], newrot[2]);
         t->scale = glm::vec3(newscale[0], newscale[1], newscale[2]);
      }
   }
   ImGui::End();
}

void GUI::DrawEntityHierarchy(ECS& ecs, EntityHandle ent) {
   ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;
   if (m_selectedEntity == ent) {
      flags |= ImGuiTreeNodeFlags_Selected;
   }

   if (ecs.GetComponent<Crimson::Transform>(ent)->children.size() == 0) {
      flags |= ImGuiTreeNodeFlags_Leaf;
   }

   if (ImGui::TreeNodeEx((EntityHandle*)ent, flags, "%s", ecs.GetComponent<Crimson::Transform>(ent)->name.c_str())) {
      if (ImGui::IsItemClicked()) {
         m_selectedEntity = ent;
         m_currentGizmoMatrix = Crimson::GetModelFromTransform(*ecs.GetComponent<Crimson::Transform>(ent));
      }

      for (EntityHandle e : ecs.GetComponent<Crimson::Transform>(ent)->children) {
         DrawEntityHierarchy(ecs, e);
      }
      ImGui::TreePop();
   } else {
      if (ImGui::IsItemClicked()) {
         m_selectedEntity = ent;
         m_currentGizmoMatrix = Crimson::GetModelFromTransform(*ecs.GetComponent<Crimson::Transform>(ent));
      }
   }
}

void GUI::DrawGizmos(ECS& ecs, Crimson::SceneManager& sceneManager, Crimson::Camera& camera) {
   int x, y;
   SDL_GetWindowPosition(m_window, &x, &y);

   float matrixTranslation[3], matrixRotation[3], matrixScale[3];
	ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(m_currentGizmoMatrix), matrixTranslation, matrixRotation, matrixScale);

   if (m_selectedEntity) {
      ecs.GetComponent<Crimson::Transform>(m_selectedEntity)->position = glm::vec3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
      ecs.GetComponent<Crimson::Transform>(m_selectedEntity)->rotation = glm::vec3(glm::radians(matrixRotation[0]), glm::radians(matrixRotation[1]), glm::radians(matrixRotation[2]));
      ecs.GetComponent<Crimson::Transform>(m_selectedEntity)->scale = glm::vec3(matrixScale[0], matrixScale[1], matrixScale[2]);

      ImGuizmo::Enable(true);

      ImGuiIO& io = ImGui::GetIO();
      ImGuizmo::SetRect(x, y, io.DisplaySize.x, io.DisplaySize.y);
      ImGuizmo::Manipulate(glm::value_ptr(camera.GetView()), glm::value_ptr(camera.GetProjection()), m_currentGizmoOperation, m_currentGizmoMode, glm::value_ptr(m_currentGizmoMatrix), NULL, NULL);
   } else {
      ImGuizmo::Enable(false);
   }
}

void GUI::DrawMainMenuBar(Crimson::SceneManager& sceneManager, ECS& ecs) {
   if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("File")) {
         if (ImGui::MenuItem("Open")) {
            m_showOpen = true;
         }

         if (ImGui::MenuItem("Save")) {
            if (!m_currentScenePath.empty()) {
               sceneManager.Serialize(m_currentScenePath, ecs);
            } else {
               m_showSaveAs = true;
            }
         }
         if (ImGui::MenuItem("Save As")) {
            m_showSaveAs = true;
         }
         ImGui::End();
      }

      if (ImGui::BeginMenu("Windows")) {
         ImGui::MenuItem("Hierarchy", NULL, &m_hierarchyOpen);
         ImGui::MenuItem("Inspector", NULL, &m_inspectorOpen);
         ImGui::End();
      }

      ImGui::EndMainMenuBar();
   }

   if (m_showSaveAs) {
      ImGui::OpenPopup("Save Scene As");
      m_showSaveAs = false;
   }

   if (m_showOpen) {
      ImGui::OpenPopup("Open Scene");
      m_showOpen = false;
   }

   if (m_fileDialog.showFileDialog("Save Scene As", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(700, 310), ".scene,*.*"))
   {
      sceneManager.Serialize(m_fileDialog.selected_path, ecs);

      m_currentScenePath = m_fileDialog.selected_path;
   }

   if (m_fileDialog.showFileDialog("Open Scene", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".scene,*.*"))
   {
      m_selectedEntity = 0;
      if (sceneManager.Deserialize(m_fileDialog.selected_path, ecs)) {
         m_currentScenePath = m_fileDialog.selected_path;
      }
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
   m_window = window;

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

   DrawGizmos(ecs, sceneManager, camera);

   DrawMainMenuBar(sceneManager, ecs);

   if (m_hierarchyOpen) {DrawHierarchy(ecs, sceneManager);}
   if (m_inspectorOpen) {DrawInspector(ecs, sceneManager);}

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
