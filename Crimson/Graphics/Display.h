#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL.h>

namespace Crimson {
   class Display {
   private:
      SDL_Window* m_window;
      SDL_GLContext m_glContext;
   public:
      Display() {}
      ~Display();
      void Init(int width, int height, int flags, const char* title);
      void Clear(float r, float g, float b, float a);
      void Present();
   };
}
#endif /* end of include guard: DISPLAY_H */
