#ifndef GUI_H
#define GUI_H

#include <SDL.h>
#include <Crimson.h>

#include <Utils/FileBrowser/ImGuiFileBrowser.h>

class GUI {
private:
   imgui_addons::ImGuiFileBrowser m_fileDialog;

   std::string m_workingDir;

   bool m_hierarchyOpen{true};
   bool m_inspectorOpen{true};

   EntityHandle m_selectedEntity {0};

   void DrawHierarchy(ECS& ecs, Crimson::SceneManager& sceneManager);
   void DrawInspector(ECS& ecs, Crimson::SceneManager& sceneManager);

   void DrawEntityHierarchy(ECS& ecs, EntityHandle ent);

   void DrawMainMenuBar();
public:
   GUI(){}
   GUI(SDL_Window* window, const SDL_GLContext glContext);
   void Init(SDL_Window* window, const SDL_GLContext glContext);
   void Render(SDL_Window* window, ECS& ecs, Crimson::SceneManager& sceneManager, Crimson::Camera& camera);
   void Update(const SDL_Event& event);
   ~GUI();
};

#endif /* end of include guard: GUI_H */
