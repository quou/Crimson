#ifndef GUI_H
#define GUI_H

#include <SDL.h>
#include <Crimson.h>

#include <Utils/FileBrowser/ImGuiFileBrowser.h>
#include <Utils/TextEditor/TextEditor.h>
#include <Utils/ImGuizmo.h>

#include <sstream>

struct FileNode {
   bool isDir{false};

   std::string name;
   std::string absPath;
   std::string ext;

   std::vector<FileNode> children;
   bool hasparent{false};
};

class GUI {
private:
   imgui_addons::ImGuiFileBrowser m_fileDialog;

   std::string m_workingDir;

   bool m_hierarchyOpen{true};
   bool m_inspectorOpen{true};
   bool m_consoleOpen{true};
   bool m_projectOpen{true};
   bool m_sceneSettingsOpen{true};
   bool m_toolbarOpen{true};
   bool m_sceneOpen{true};
   bool m_gameOpen{true};

   bool m_showSaveAs{false};
   bool m_showOpen{false};

   bool m_oldSceneFocused{false};

	ImGuizmo::MODE m_currentGizmoMode{ImGuizmo::WORLD};
   ImGuizmo::OPERATION m_currentGizmoOperation{ImGuizmo::TRANSLATE};

   Crimson::EntityHandle m_selectedEntity {0};

   glm::mat4 m_currentGizmoMatrix;

   void DrawHierarchy(Crimson::ECS& ecs, Crimson::SceneManager& sceneManager);
   void DrawInspector(Crimson::ECS& ecs, Crimson::SceneManager& sceneManager);
   void DrawProject(Crimson::ECS& ecs, Crimson::SceneManager& sceneManager);
   void DrawSceneSettings(Crimson::ECS& ecs, Crimson::SceneManager& sceneManager);
   void DrawToolbar(Crimson::ECS& ecs, Crimson::SceneManager& sceneManager);

   void DrawEntityHierarchy(Crimson::ECS& ecs, Crimson::EntityHandle ent);
   void DrawDir(const std::string& dir);

   void DrawGizmos(Crimson::ECS& ecs, Crimson::SceneManager& sceneManager, Crimson::Camera& camera, Crimson::RenderTarget& renderTarget);

   void DrawMainMenuBar(Crimson::SceneManager& sceneManager, Crimson::ECS& ecs);

   std::string m_currentScenePath;

   SDL_Window* m_window;

   ImVec2 m_sceneWindowPos;

   bool m_isSceneFocused;

   float m_newpos[3];
   float m_newrot[3];
   float m_newscale[3];

   bool m_isSaved{false};

   bool m_shouldPlay {false};

   TextEditor m_textEditor;
public:
   GUI(){}
   GUI(SDL_Window* window, const SDL_GLContext glContext);
   void Init(SDL_Window* window, const SDL_GLContext glContext);

   void EndFrame();

   void Render(SDL_Window* window, Crimson::ECS& ecs, Crimson::SceneManager& sceneManager, Crimson::Camera& camera, Crimson::RenderTarget& sceneRenderTarget, Crimson::RenderTarget& gameRenderTarget);
   void Update(const SDL_Event& event);

   void DrawConsole(std::ostringstream& strCout);
   void DrawScene(Crimson::ECS& ecs, Crimson::RenderTarget& renderTarget, Crimson::Camera& camera);
   void DrawGame(Crimson::ECS& ecs, Crimson::RenderTarget& renderTarget, Crimson::Camera& camera);

   void OpenScene(const std::string& fileName, Crimson::SceneManager& sceneManager, Crimson::ECS& ecs);
   void SaveScene(Crimson::SceneManager& sceneManager, Crimson::ECS& ecs);

   inline std::string GetCurrentScenePath() {return m_currentScenePath;}

   inline bool IsSceneFocused() {return m_isSceneFocused;}
   inline bool GetShouldPlay() {return m_shouldPlay;}
   inline bool IsSaved() {return m_isSaved;}

   ~GUI();
};

#endif /* end of include guard: GUI_H */
