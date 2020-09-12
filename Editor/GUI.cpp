#include "GUI.h"

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <filesystem>
#include <string>
#include <algorithm>


static bool hasEnding(std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

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
      char buf[256];
      strcpy(buf, ecs.GetComponent<Crimson::Transform>(m_selectedEntity)->name.c_str());
      ImGui::InputText("Name", buf, 256);
      ecs.GetComponent<Crimson::Transform>(m_selectedEntity)->name = buf;

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
         m_newpos[0] = t->position.x;m_newpos[1] = t->position.y;m_newpos[2] = t->position.z;
         m_newrot[0] = t->rotation.x;m_newrot[1] = t->rotation.y;m_newrot[2] = t->rotation.z;
         m_newscale[0] = t->scale.x;m_newscale[1] = t->scale.y;m_newscale[2] = t->scale.z;

         ImGui::InputFloat3("Position", m_newpos);
         ImGui::InputFloat3("Rotation", m_newrot);
         ImGui::InputFloat3("Scale", m_newscale);

         t->position = glm::vec3(m_newpos[0], m_newpos[1], m_newpos[2]);
         t->rotation = glm::vec3(m_newrot[0], m_newrot[1], m_newrot[2]);
         t->scale = glm::vec3(m_newscale[0], m_newscale[1], m_newscale[2]);
      }

      if (ecs.HasComponent<Crimson::ScriptComponent>(m_selectedEntity)) {
         if (ImGui::CollapsingHeader("Script")) {
            ImGui::Text("%s", ecs.GetComponent<Crimson::ScriptComponent>(m_selectedEntity)->scriptFile.c_str());
            if (ImGui::BeginDragDropTarget()) {
               if (ImGuiPayload const* payload = ImGui::AcceptDragDropPayload("File")) {
                  std::string toSet = static_cast<const char*>(payload->Data);
                  if (hasEnding(toSet, ".chai")) {
                     ecs.GetComponent<Crimson::ScriptComponent>(m_selectedEntity)->scriptFile = toSet;
                  } else {
                     std::cout << "Invalid script file. Only use .chai files for scripts" << '\n';
                  }
               }
               ImGui::EndDragDropTarget();
            }
         }
      }

      if (ecs.HasComponent<Crimson::ModelComponent>(m_selectedEntity)) {
         if (ImGui::CollapsingHeader("Model")) {
            ImGui::Text("Mesh");
            if (ImGui::BeginDragDropTarget()) {
               if (ImGuiPayload const* payload = ImGui::AcceptDragDropPayload("File")) {
                  std::string toSet = static_cast<const char*>(payload->Data);
                  if (hasEnding(toSet, ".obj")) {
                     ecs.GetComponent<Crimson::ModelComponent>(m_selectedEntity)->model.Load(toSet);
                  } else {
                     std::cout << "Invalid mesh file. Only use .obj files for meshes" << '\n';
                  }
               }
               ImGui::EndDragDropTarget();
            }

            ImGui::Text("Texture");
            if (ImGui::BeginDragDropTarget()) {
               if (ImGuiPayload const* payload = ImGui::AcceptDragDropPayload("File")) {
                  std::string toSet = static_cast<const char*>(payload->Data);
                  ecs.GetComponent<Crimson::ModelComponent>(m_selectedEntity)->texture.Load(toSet);
               }
               ImGui::EndDragDropTarget();
            }

            if (ImGui::TreeNode("Material")) {

               ImGui::Text("Vertex Shader");
               if (ImGui::BeginDragDropTarget()) {
                  if (ImGuiPayload const* payload = ImGui::AcceptDragDropPayload("File")) {
                     std::string toSet = static_cast<const char*>(payload->Data);
                     ecs.GetComponent<Crimson::ModelComponent>(m_selectedEntity)->shader.Init(toSet, ecs.GetComponent<Crimson::ModelComponent>(m_selectedEntity)->shader.GetFragPath());
                  }
                  ImGui::EndDragDropTarget();
               }
               ImGui::SameLine();
               ImGui::Text("Fragment Shader");
               if (ImGui::BeginDragDropTarget()) {
                  if (ImGuiPayload const* payload = ImGui::AcceptDragDropPayload("File")) {
                     std::string toSet = static_cast<const char*>(payload->Data);
                     ecs.GetComponent<Crimson::ModelComponent>(m_selectedEntity)->shader.Init(ecs.GetComponent<Crimson::ModelComponent>(m_selectedEntity)->shader.GetVertPath(), toSet);

                  }
                  ImGui::EndDragDropTarget();
               }

               Crimson::Material* mat = &ecs.GetComponent<Crimson::ModelComponent>(m_selectedEntity)->material;

               float newCol[3] = {mat->ambient.x, mat->ambient.y, mat->ambient.z};
               ImGui::ColorEdit3("Ambient Color", newCol);
               mat->ambient = glm::vec3(newCol[0], newCol[1], newCol[2]);

               float newCol2[3] = {mat->diffuse.x, mat->diffuse.y, mat->diffuse.z};
               ImGui::ColorEdit3("Diffuse Color", newCol2);
               mat->diffuse = glm::vec3(newCol2[0], newCol2[1], newCol2[2]);

               float newCol3[3] = {mat->specular.x, mat->specular.y, mat->specular.z};
               ImGui::ColorEdit3("Specular Color", newCol3);
               mat->specular = glm::vec3(newCol3[0], newCol3[1], newCol3[2]);

               ImGui::InputFloat("Shininess", &mat->shininess);

               ImGui::TreePop();
            }
         }
      }

      if (ecs.HasComponent<Crimson::PointLight>(m_selectedEntity)) {
         if (ImGui::CollapsingHeader("Point Light")) {
            Crimson::PointLight* l = ecs.GetComponent<Crimson::PointLight>(m_selectedEntity);

            ImGui::InputFloat("Constant", &l->constant);
            ImGui::InputFloat("Linear", &l->linear);
            ImGui::InputFloat("Quadratic", &l->quadratic);

            float newCol[3] = {l->ambient.x, l->ambient.y, l->ambient.z};
            ImGui::ColorEdit3("Ambient Color", newCol);
            l->ambient = glm::vec3(newCol[0], newCol[1], newCol[2]);

            float newCol2[3] = {l->diffuse.x, l->diffuse.y, l->diffuse.z};
            ImGui::ColorEdit3("Diffuse Color", newCol2);
            l->diffuse = glm::vec3(newCol2[0], newCol2[1], newCol2[2]);

            float newCol3[3] = {l->specular.x, l->specular.y, l->specular.z};
            ImGui::ColorEdit3("Specular Color", newCol3);
            l->specular = glm::vec3(newCol3[0], newCol3[1], newCol3[2]);
         }
      }
   }
   ImGui::End();
}

void GUI::DrawToolbar(ECS& ecs, Crimson::SceneManager& sceneManager) {
   ImGui::Begin("Toolbox", &m_toolbarOpen);

   ImGui::Columns(3, NULL, false);

   ImGui::Text("Entities");

   if (ImGui::MenuItem("Delete Selected")) {
      if (m_selectedEntity) {
         sceneManager.DeleteEntity(m_selectedEntity, ecs);
         m_selectedEntity = 0;
      }
   }

   if (ImGui::MenuItem("Create New")) {
      sceneManager.CreateEntity("New Entity", ecs);
      m_selectedEntity = 0;
   }

   ImGui::NextColumn();

   ImGui::Text("Scene");
   if (ImGui::Button("Play")) {m_shouldPlay = true;}
   if (ImGui::Button("stop")) {m_shouldPlay = false;}

   ImGui::NextColumn();

   ImGui::Columns(1);

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
      if (ImGui::BeginDragDropTarget()) {
         if (ImGuiPayload const* payload = ImGui::AcceptDragDropPayload("Reparent")) {
            EntityHandle draggedEntity = *static_cast<EntityHandle*>(payload->Data);

            if (ecs.GetComponent<Crimson::Transform>(draggedEntity)->parent) {
               Crimson::RemoveChild(ecs.GetComponent<Crimson::Transform>(ecs.GetComponent<Crimson::Transform>(draggedEntity)->parent), draggedEntity);
               ecs.GetComponent<Crimson::Transform>(draggedEntity)->parent = 0;
            }

            Crimson::AddChild(ecs.GetComponent<Crimson::Transform>(ent), draggedEntity);
            Crimson::AddParent(ecs.GetComponent<Crimson::Transform>(draggedEntity), ent);
         }
         ImGui::EndDragDropTarget();
      }
      if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
         ImGui::Text("%s", ecs.GetComponent<Crimson::Transform>(ent)->name.c_str());

         ImGui::SetDragDropPayload("Reparent", &ent, sizeof(ent));

         ImGui::EndDragDropSource();
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
      if (ImGui::BeginDragDropTarget()) {
         if (ImGuiPayload const* payload = ImGui::AcceptDragDropPayload("Reparent")) {
            EntityHandle draggedEntity = *static_cast<EntityHandle*>(payload->Data);

            if (ecs.GetComponent<Crimson::Transform>(draggedEntity)->parent) {
               Crimson::RemoveChild(ecs.GetComponent<Crimson::Transform>(ecs.GetComponent<Crimson::Transform>(draggedEntity)->parent), draggedEntity);
               ecs.GetComponent<Crimson::Transform>(draggedEntity)->parent = 0;
            }

            Crimson::AddChild(ecs.GetComponent<Crimson::Transform>(ent), draggedEntity);
            Crimson::AddParent(ecs.GetComponent<Crimson::Transform>(draggedEntity), ent);
         }
         ImGui::EndDragDropTarget();
      }
      if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
         ImGui::Text("%s", ecs.GetComponent<Crimson::Transform>(ent)->name.c_str());

         ImGui::SetDragDropPayload("Reparent", &ent, sizeof(ent));

         ImGui::EndDragDropSource();
      }
   }
}

void GUI::DrawGizmos(ECS& ecs, Crimson::SceneManager& sceneManager, Crimson::Camera& camera, Crimson::RenderTarget& renderTarget) {
   if (m_selectedEntity) {
      ImGuizmo::Enable(true);
      Crimson::Transform* t = ecs.GetComponent<Crimson::Transform>(m_selectedEntity);

      float matrixTranslation[3], matrixRotation[3], matrixScale[3];
      ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(m_currentGizmoMatrix), matrixTranslation, matrixRotation, matrixScale);

      t->worldPosition = glm::vec3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
      t->rotation = glm::vec3(matrixRotation[0], matrixRotation[1], matrixRotation[2]);
      t->scale = glm::vec3(matrixScale[0], matrixScale[1], matrixScale[2]);

      if (t->parent) {
         t->position = t->worldPosition - ecs.GetComponent<Crimson::Transform>(t->parent)->worldPosition;
      } else {
         t->position = t->worldPosition;
      }

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
               m_isSaved = true;
            } else {
               m_showSaveAs = true;
               m_isSaved = false;
            }
         }
         if (ImGui::MenuItem("Save As")) {
            m_showSaveAs = true;
         }
         ImGui::End();
      }

      if (ImGui::BeginMenu("Windows")) {
         ImGui::MenuItem("Console", NULL, &m_consoleOpen);
         ImGui::MenuItem("Hierarchy", NULL, &m_hierarchyOpen);
         ImGui::MenuItem("Inspector", NULL, &m_inspectorOpen);
         ImGui::MenuItem("Project Explorer", NULL, &m_projectOpen);
         ImGui::MenuItem("Scene Config", NULL, &m_sceneSettingsOpen);
         ImGui::MenuItem("Toolbar", NULL, &m_toolbarOpen);
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
      m_isSaved = false;

      m_currentScenePath = m_fileDialog.selected_path;
   }

   if (m_fileDialog.showFileDialog("Open Scene", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".scene,*.*"))
   {
      OpenScene(m_fileDialog.selected_path, sceneManager, ecs);
   }
}

void GUI::SaveScene(Crimson::SceneManager& sceneManager, ECS& ecs) {
   if (!m_currentScenePath.empty()) {
      sceneManager.Serialize(m_currentScenePath, ecs);
      m_isSaved = true;
   } else {
      m_showSaveAs = true;
      m_isSaved = false;
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
   io.ConfigWindowsMoveFromTitleBarOnly = true;

   ImGuiStyle& style = ImGui::GetStyle();
   if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
   {
     style.WindowRounding = 0.0f;
     style.Colors[ImGuiCol_WindowBg].w = 1.0f;
   }

   ImGui_ImplSDL2_InitForOpenGL(window, glContext);
   ImGui_ImplOpenGL3_Init("#version 330 core");

   auto lang = TextEditor::LanguageDefinition::AngelScript();
   m_textEditor.SetLanguageDefinition(lang);
   static const char* fileToEdit = "Resources/Scripts/TestScript.as";
	{
		std::ifstream t(fileToEdit);
		if (t.good())
		{
			std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
			m_textEditor.SetText(str);
		}
	}
}

void GUI::Render(SDL_Window* window, ECS& ecs, Crimson::SceneManager& sceneManager, Crimson::Camera& camera, Crimson::RenderTarget& renderTarget) {
   m_isSaved = false;

   ImGui_ImplOpenGL3_NewFrame();
   ImGui_ImplSDL2_NewFrame(window);
   ImGui::NewFrame();
   ImGuizmo::BeginFrame();

   ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_PassthruCentralNode);

   if (m_inspectorOpen) {DrawInspector(ecs, sceneManager);}

   if (m_isSceneFocused) {
      DrawGizmos(ecs, sceneManager, camera, renderTarget);
   }

   DrawMainMenuBar(sceneManager, ecs);

   if (m_hierarchyOpen) {DrawHierarchy(ecs, sceneManager);}
   if (m_projectOpen) {DrawProject(ecs, sceneManager);}
   if (m_sceneSettingsOpen) {DrawSceneSettings(ecs, sceneManager);}
   if (m_toolbarOpen) {DrawToolbar(ecs, sceneManager);}

   // ImGui::Begin("Code Editor");
   // m_textEditor.Render("Code Editor");
   // ImGui::End();

//   DrawConsole(strCout);
   DrawScene(ecs, renderTarget, camera);
}

void GUI::EndFrame() {
   ImGui::Render();
   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
   ImGuiIO& io = ImGui::GetIO(); (void)io;
   if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
      SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
   }
}

void GUI::OpenScene(const std::string& fileName, Crimson::SceneManager& sceneManager, ECS& ecs) {
   m_selectedEntity = 0;
   if (sceneManager.Deserialize(fileName, ecs)) {
      m_currentScenePath = fileName;
   }
}

GUI::~GUI() {
   ImGui_ImplOpenGL3_Shutdown();
   ImGui_ImplSDL2_Shutdown();
   ImGui::DestroyContext();
}

void GUI::DrawConsole(std::ostringstream& strCout) {
   if (m_consoleOpen) {
      ImGui::Begin("Console", &m_consoleOpen);
      if (ImGui::Button("Clear")) {
         strCout.str(std::string());
      }
      ImGui::TextWrapped("%s", strCout.str().c_str());
      ImGui::End();
   }
}

void GUI::DrawScene(ECS& ecs, Crimson::RenderTarget& renderTarget, Crimson::Camera& camera) {
   ImGui::Begin("Scene");
   if (ImGui::IsWindowFocused()) {
      m_isSceneFocused = true;
   } else {
      m_isSceneFocused = false;
   }

   m_sceneWindowPos = ImGui::GetWindowPos();
   renderTarget.Resize(ImGui::GetWindowSize().x-15, ImGui::GetWindowSize().y-35);
   ImGui::Image((ImTextureID)renderTarget.GetOutput(), ImVec2(renderTarget.GetWidth(), renderTarget.GetHeight()), ImVec2(0, 1), ImVec2(1, 0));

   ImGuiIO& io = ImGui::GetIO();
   ImGuizmo::SetDrawlist();
   ImGuizmo::SetRect(m_sceneWindowPos.x, m_sceneWindowPos.y, renderTarget.GetWidth(), renderTarget.GetHeight());
   ImGuizmo::Manipulate(glm::value_ptr(camera.GetView()), glm::value_ptr(camera.GetProjection()), m_currentGizmoOperation, m_currentGizmoMode, glm::value_ptr(m_currentGizmoMatrix), NULL, NULL);

   ImGui::End();
}

static void eraseSubStr(std::string & mainStr, const std::string & toErase)
{
    // Search for the substring in string
    size_t pos = mainStr.find(toErase);
    if (pos != std::string::npos)
    {
        // If found then erase it from string
        mainStr.erase(pos, toErase.length());
    }
}

void GUI::DrawDir(const std::string& dir) {
   for (const auto& entry : std::filesystem::directory_iterator(dir)) {
      ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;

      bool isDir = entry.is_directory();

      if (!isDir) {
         flags |= ImGuiTreeNodeFlags_Leaf;
      }

      if (ImGui::TreeNodeEx(entry.path().filename().c_str(), flags, "%s", entry.path().filename().c_str())) {
         if (isDir) {
            DrawDir(dir + std::string(entry.path().filename()));
         } else {
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
               std::string dragString = entry.path().string();
               std::replace(dragString.begin(), dragString.end(), '\\', '/');
               eraseSubStr(dragString, m_workingDir);

               ImGui::Text("%s", entry.path().filename().c_str());

               ImGui::SetDragDropPayload("File", dragString.data(), dragString.size() + 1);

               ImGui::EndDragDropSource();
            }
         }

         ImGui::TreePop();
      }
   }
}

void GUI::DrawProject(ECS& ecs, Crimson::SceneManager& sceneManager) {
   ImGui::Begin("Project Explorer", &m_projectOpen);

   if (ImGui::TreeNode("Resources")) {
      DrawDir(m_workingDir + "Resources/");
      ImGui::TreePop();
   }

   ImGui::End();
}

void GUI::DrawSceneSettings(ECS& ecs, Crimson::SceneManager& sceneManager) {
   ImGui::Begin("Scene Config", &m_sceneSettingsOpen);

   if (ImGui::CollapsingHeader("Lighting")) {
      if (ImGui::TreeNode("Directional Light")) {
         float newdir[3] = {sceneManager.GetConfig()->directionalLight.direction.x, sceneManager.GetConfig()->directionalLight.direction.y, sceneManager.GetConfig()->directionalLight.direction.z};
         ImGui::DragFloat3("Direction", newdir, 0.01f, -1.0f, 1.0f);
         sceneManager.GetConfig()->directionalLight.direction = glm::vec3(newdir[0], newdir[1], newdir[2]);

         float newCol[3] = {sceneManager.GetConfig()->directionalLight.ambient.x, sceneManager.GetConfig()->directionalLight.ambient.y, sceneManager.GetConfig()->directionalLight.ambient.z};
         ImGui::ColorEdit3("Ambient Color", newCol);
         sceneManager.GetConfig()->directionalLight.ambient = glm::vec3(newCol[0], newCol[1], newCol[2]);

         float newCol2[3] = {sceneManager.GetConfig()->directionalLight.diffuse.x, sceneManager.GetConfig()->directionalLight.diffuse.y, sceneManager.GetConfig()->directionalLight.diffuse.z};
         ImGui::ColorEdit3("Diffuse Color", newCol2);
         sceneManager.GetConfig()->directionalLight.diffuse = glm::vec3(newCol2[0], newCol2[1], newCol2[2]);

         float newCol3[3] = {sceneManager.GetConfig()->directionalLight.specular.x, sceneManager.GetConfig()->directionalLight.specular.y, sceneManager.GetConfig()->directionalLight.specular.z};
         ImGui::ColorEdit3("Specular Color", newCol3);
         sceneManager.GetConfig()->directionalLight.specular = glm::vec3(newCol3[0], newCol3[1], newCol3[2]);

         ImGui::TreePop();
      }
   }

   ImGui::End();
}
