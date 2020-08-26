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

   protected:
      Application();
      virtual void OnBegin() {}
      virtual void OnUpdate(float delta) {}
      virtual void OnRender(float delta) {}

      Keyboard m_keyboard;

   public:
      void Run();

      virtual ~Application();
   };
}

#endif /* end of include guard: APPLICATION_H */
