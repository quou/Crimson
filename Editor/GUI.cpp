#include "GUI.h"

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Utils/ImGuizmo.h>

#include <filesystem>

static std::string EraseSubstring(const std::string & main, const std::string & erase)
{
   std::string result = main;

    // Search for the substring in string
    size_t pos = main.find(erase);
    if (pos != std::string::npos)
    {
        // If found then erase it from string
        result.erase(pos, erase.length());
    }
    return result;
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
   std::cout << m_workingDir << std::endl;
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

   /* Scene Hierarchy */
   ImGui::Begin("Hierarchy");
   if (ImGui::Button("Create Entity")) {
      sceneManager.CreateEntity("New Entity", ecs);
   } ImGui::SameLine();
   if (ImGui::Button("Delete Entity")) {
      if (m_selectedEntity != 0) {
         sceneManager.DeleteEntity(m_selectedEntity, ecs);
         m_selectedEntity = 0;
      }
   }
   ImGui::Separator();

   std::vector<EntityHandle> ents = sceneManager.GetEntities();

   static ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
   static glm::mat4 currentGizmoMatrix = Crimson::GetModelFromTransform(*ecs.GetComponent<Crimson::Transform>(m_selectedEntity));
   for (unsigned int i = 0; i < ents.size(); i++) {
      ImGuiTreeNodeFlags flags = baseFlags;
      if (ents[i] == m_selectedEntity) {
         flags |= ImGuiTreeNodeFlags_Selected;
      }
      if (ImGui::TreeNodeEx((void*)(EntityHandle)ents[i], flags, "%s", ecs.GetComponent<Crimson::Transform>(ents[i])->name.c_str())) {
         ImGui::TreePop();
      }
      if (ImGui::IsItemClicked()) {
         m_selectedEntity = ents[i];
         currentGizmoMatrix = Crimson::GetModelFromTransform(*ecs.GetComponent<Crimson::Transform>(m_selectedEntity));
      }

   }
   ImGui::End();

   ImGuizmo::Enable(true);

   ImGui::Begin("Inspector");
   static bool useSnap(false);
   static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);

   ImGui::Text("Gizmo mode"); ImGui::SameLine();
   if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
		mCurrentGizmoMode = ImGuizmo::WORLD;
	ImGui::SameLine();
	if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
		mCurrentGizmoMode = ImGuizmo::LOCAL;

	if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
		mCurrentGizmoOperation = ImGuizmo::SCALE;


   float matrixTranslation[3] = {ecs.GetComponent<Crimson::Transform>(m_selectedEntity)->position.x,ecs.GetComponent<Crimson::Transform>(m_selectedEntity)->position.y,ecs.GetComponent<Crimson::Transform>(m_selectedEntity)->position.z};
   float matrixRotation[3] = {ecs.GetComponent<Crimson::Transform>(m_selectedEntity)->rotation.x,ecs.GetComponent<Crimson::Transform>(m_selectedEntity)->rotation.y,ecs.GetComponent<Crimson::Transform>(m_selectedEntity)->rotation.z};
   float matrixScale[3] = {ecs.GetComponent<Crimson::Transform>(m_selectedEntity)->scale.x,ecs.GetComponent<Crimson::Transform>(m_selectedEntity)->scale.y,ecs.GetComponent<Crimson::Transform>(m_selectedEntity)->scale.z};
	ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(currentGizmoMatrix), matrixTranslation, matrixRotation, matrixScale);

   char* buf = strdup(ecs.GetComponent<Crimson::Transform>(m_selectedEntity)->name.c_str());
   ImGui::InputText("Name", buf, 256);
   ecs.GetComponent<Crimson::Transform>(m_selectedEntity)->name = buf;

   if (ImGui::CollapsingHeader("Transform")) {
	  ImGui::InputFloat3("Position", matrixTranslation, 3);
	  ImGui::InputFloat3("Rotation", matrixRotation, 3);
	  ImGui::InputFloat3("Scale", matrixScale, 3);
   }

   if (ecs.HasComponent<Crimson::ModelComponent>(m_selectedEntity)) {
      if (ImGui::CollapsingHeader("Model")) {
         ImGui::Text("Mesh: %s", ecs.GetComponent<Crimson::ModelComponent>(m_selectedEntity)->model.GetRes().c_str()); ImGui::SameLine();
         if (ImGui::Button("Load Mesh")) {
            m_selectMeshPopup = true;
         }

         ImGui::Text("Texture: %s", ecs.GetComponent<Crimson::ModelComponent>(m_selectedEntity)->texture.GetRes().c_str()); ImGui::SameLine();
         if (ImGui::Button("Load Texture")) {
            m_selectTexturePopup = true;
         }
      }
   }

	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, glm::value_ptr(currentGizmoMatrix));
   ecs.GetComponent<Crimson::Transform>(m_selectedEntity)->position = glm::vec3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
   ecs.GetComponent<Crimson::Transform>(m_selectedEntity)->rotation = glm::vec3(glm::radians(matrixRotation[0]), glm::radians(matrixRotation[1]), glm::radians(matrixRotation[2]));
   ecs.GetComponent<Crimson::Transform>(m_selectedEntity)->scale = glm::vec3(matrixScale[0], matrixScale[1], matrixScale[2]);
   ImGui::End();

   ImGuiIO& io = ImGui::GetIO(); (void)io;
   int x, y, w, h;
   SDL_GetWindowSize(window, &w, &h);
   SDL_GetWindowPosition(window, &x, &y);
	ImGuizmo::SetRect(x, y, w, h);
	ImGuizmo::Manipulate(glm::value_ptr(camera.GetView()), glm::value_ptr(camera.GetProjection()), mCurrentGizmoOperation, mCurrentGizmoMode, glm::value_ptr(currentGizmoMatrix));


   if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("File")) {
         if (ImGui::MenuItem("Save")) {
            sceneManager.Serialize("Resources/TestMap.txt", ecs);
         }
         ImGui::EndMenu();
      }

      ImGui::EndMainMenuBar();
   }

   if (m_selectMeshPopup) {
      ImGui::OpenPopup("Select Mesh");
      m_selectMeshPopup = false;
   }

   if (m_selectTexturePopup) {
      ImGui::OpenPopup("Select Texture");
      m_selectTexturePopup = false;
   }

   if (m_fileDialog.showFileDialog("Select Mesh", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".obj")) {
      std::string path = EraseSubstring(m_fileDialog.selected_path, m_workingDir);
      if (ecs.HasComponent<Crimson::ModelComponent>(m_selectedEntity)) {
         ecs.GetComponent<Crimson::ModelComponent>(m_selectedEntity)->model.Load(path);
      }
   }

   if (m_fileDialog.showFileDialog("Select Texture", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".png,.jpg")) {
      std::string path = EraseSubstring(m_fileDialog.selected_path, m_workingDir);
      if (ecs.HasComponent<Crimson::ModelComponent>(m_selectedEntity)) {
         ecs.GetComponent<Crimson::ModelComponent>(m_selectedEntity)->texture.Load(path);
      }
   }

   ImGui::Begin("Scene Control Panel");

   ImGui::SliderFloat("Ambient Intensity", &sceneManager.GetConfig()->ambientLight.m_intensity, 0.0f, 1.0f);
   static float newcol[3] = { sceneManager.GetConfig()->ambientLight.m_color.x, sceneManager.GetConfig()->ambientLight.m_color.y, sceneManager.GetConfig()->ambientLight.m_color.z};
   ImGui::ColorEdit3("Ambient Color", newcol);
   ImGui::Separator();
   ImGui::SliderFloat("Diffuse Intensity", &sceneManager.GetConfig()->directionalLight.m_intensity, 0.0f, 1.0f);
   static float newcol2[3] = { sceneManager.GetConfig()->directionalLight.m_color.x, sceneManager.GetConfig()->directionalLight.m_color.y, sceneManager.GetConfig()->directionalLight.m_color.z};
   ImGui::ColorEdit3("Diffuse Color", newcol2);

   float newDirection[3] = {sceneManager.GetConfig()->directionalLight.m_direction.x, sceneManager.GetConfig()->directionalLight.m_direction.y, sceneManager.GetConfig()->directionalLight.m_direction.z};
   ImGui::SliderFloat3("Diffuse Direction", newDirection, -1.0f, 1.0f);
   sceneManager.GetConfig()->directionalLight.m_direction = glm::vec3(newDirection[0],newDirection[1],newDirection[2]);
   sceneManager.GetConfig()->directionalLight.m_color = glm::vec3(newcol2[0],newcol2[1],newcol2[2]);
   sceneManager.GetConfig()->ambientLight.m_color = glm::vec3(newcol[0],newcol[1],newcol[2]);
   sceneManager.GetConfig()->ambientLight.m_color = glm::vec3(newcol[0],newcol[1],newcol[2]);
   ImGui::End();

   ImGui::Render();
   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
