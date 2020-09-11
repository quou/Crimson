#ifndef APPLICATION_H
#define APPLICATION_H

#include "Graphics/Display.h"
#include "Graphics/Renderer.h"
#include "Input/Keyboard.h"
#include "SceneManagement/SceneManager.h"
#include "SLECS.h"

#include <iostream>
#include <sstream>

#include <angelscript.h>

namespace Crimson {
   class Application {
   private:

      void Init();
      void Update(float delta);
      void Render();
      void Quit();

      Display m_display;

      float m_deltaTime;

      bool m_isRunning {true};

      SDL_Event m_event;

      void (*m_eventCallback)(SDL_Event event);

      std::string m_title;

   protected:
      Application(const std::string& title);
      virtual void OnBegin() {}
      virtual void OnUpdate(float delta) {}
      virtual void OnRender(float delta) {}

      inline SDL_Window* GetSDLWindow() {return m_display.GetWindow();}
      inline Display* GetDisplay() {return &m_display;}
      inline const SDL_GLContext& GetSDLGLContext() const {return m_display.GetGLContext();}

      inline const SDL_Event& GetEvent() const {return m_event;}

      Renderer m_renderer;
      Keyboard m_keyboard;
      ECS m_ecs;
      SceneManager m_sceneManager;

      asIScriptEngine* m_scriptingEngine;

   public:
      void Run();
      void SetEventCallback(void (*ptr)(SDL_Event event)) {
         m_eventCallback = ptr;
      }

      virtual ~Application();
   };
}

#endif /* end of include guard: APPLICATION_H */
