#ifndef GUI_H
#define GUI_H

#include <SDL.h>
#include <Crimson.h>

#include <Utils/FileBrowser/ImGuiFileBrowser.h>
#include <Utils/ImGuizmo.h>

#include <sstream>

class GUI {
private:
   imgui_addons::ImGuiFileBrowser m_fileDialog;

   std::string m_workingDir;

   bool m_hierarchyOpen{true};
   bool m_inspectorOpen{true};
   bool m_consoleOpen{true};
   bool m_projectOpen{true};
   bool m_sceneSettingsOpen{true};

   bool m_showSaveAs{false};
   bool m_showOpen{false};

	ImGuizmo::MODE m_currentGizmoMode{ImGuizmo::WORLD};
   ImGuizmo::OPERATION m_currentGizmoOperation{ImGuizmo::TRANSLATE};

   EntityHandle m_selectedEntity {0};

   glm::mat4 m_currentGizmoMatrix;

   void DrawHierarchy(ECS& ecs, Crimson::SceneManager& sceneManager);
   void DrawInspector(ECS& ecs, Crimson::SceneManager& sceneManager);
   void DrawProject(ECS& ecs, Crimson::SceneManager& sceneManager);
   void DrawSceneSettings(ECS& ecs, Crimson::SceneManager& sceneManager);

   void DrawEntityHierarchy(ECS& ecs, EntityHandle ent);

   void DrawGizmos(ECS& ecs, Crimson::SceneManager& sceneManager, Crimson::Camera& camera, Crimson::RenderTarget& renderTarget);

   void DrawMainMenuBar(Crimson::SceneManager& sceneManager, ECS& ecs);

   std::string m_currentScenePath;

   SDL_Window* m_window;

   ImVec2 m_sceneWindowPos;

   bool m_isSceneFocused;
public:
   GUI(){}
   GUI(SDL_Window* window, const SDL_GLContext glContext);
   void Init(SDL_Window* window, const SDL_GLContext glContext);
   void Render(SDL_Window* window, ECS& ecs, Crimson::SceneManager& sceneManager, Crimson::Camera& camera, std::ostringstream& strCout, Crimson::RenderTarget& renderTarget);
   void Update(const SDL_Event& event);

   void DrawConsole(std::ostringstream& strCout);
   void DrawScene(Crimson::RenderTarget& renderTarget, Crimson::Camera& camera);

   void OpenScene(const std::string& fileName, Crimson::SceneManager& sceneManager, ECS& ecs);

   inline bool IsSceneFocused() {return m_isSceneFocused;}

   ~GUI();
};

#endif /* end of include guard: GUI_H */
