#ifndef APPLICATION_H
#define APPLICATION_H

#include "Graphics/Display.h"
#include "Graphics/Renderer.h"
#include "Input/Keyboard.h"

namespace Crimson {
   class Application {
   private:
      void Init();
      void Update(float delta);
      void Render();
      void Quit();

      Display m_display;
      Renderer m_renderer;

      float m_deltaTime;

      bool m_isRunning {true};

      SDL_Event m_event;

   protected:
      Application();
      virtual void OnBegin() {}
      virtual void OnUpdate(float delta) {}
      virtual void OnRender(float delta) {}

      inline SDL_Window* GetSDLWindow() {return m_display.GetWindow();}
      inline Display* GetDisplay() {return &m_display;}
      inline const SDL_GLContext& GetSDLGLContext() const {return m_display.GetGLContext();}

      inline const SDL_Event& GetEvent() const {return m_event;}

      Keyboard m_keyboard;

   public:
      void Run();

      virtual ~Application();
   };
}

#endif /* end of include guard: APPLICATION_H */
