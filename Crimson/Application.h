#ifndef APPLICATION_H
#define APPLICATION_H

#include "Graphics/Display.h"

namespace Crimson {
   class Application {
   private:
      void Init();
      void Update(float delta);
      void Render();
      void Quit();

      Display m_display;

      bool m_isRunning {true};

   protected:
      virtual void OnUpdate(float delta) {}

   public:
      void Run();

      virtual ~Application();
   };
}

#endif /* end of include guard: APPLICATION_H */
