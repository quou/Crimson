#ifndef GUI_H
#define GUI_H

#include <SDL.h>
#include <Crimson.h>

#include <Utils/FileBrowser/ImGuiFileBrowser.h>

class GUI {
private:
   EntityHandle m_selectedEntity = 0;

   imgui_addons::ImGuiFileBrowser m_fileDialog;
   
   std::string m_workingDir;


   void DrawEntityHierarchy(EntityHandle entity, ECS& ecs, ImGuiTreeNodeFlags flags);
public:
   GUI(){}
   GUI(SDL_Window* window, const SDL_GLContext glContext);
   void Init(SDL_Window* window, const SDL_GLContext glContext);
   void Render(SDL_Window* window, ECS& ecs, Crimson::SceneManager& sceneManager, Crimson::Camera& camera);
   void Update(const SDL_Event& event);
   ~GUI();
};

#endif /* end of include guard: GUI_H */
