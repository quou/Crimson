#ifndef APPLICATION_H
#define APPLICATION_H

#include "Graphics/Display.h"
#include "Graphics/Renderer.h"

namespace Crimson {
   class Application {
   private:
      void Init();
      void Update(float delta);
      void Render();
      void Quit();

      Display m_display;
      Renderer m_renderer;

      bool m_isRunning {true};

   protected:
      virtual void OnUpdate(float delta) {}
      virtual void OnBegin() {}

   public:
      void Run();

      virtual ~Application();
   };
}

#endif /* end of include guard: APPLICATION_H */
