#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL.h>
#include <tuple>

namespace Crimson {
   class Display {
   private:
      SDL_Window* m_window;
      SDL_GLContext m_glContext;

      int m_width, m_height;
   public:
      Display() {}
      ~Display();
      void Init(int width, int height, int flags, const char* title);
      void Present();

      inline const SDL_GLContext& GetGLContext() const { return m_glContext; }
      inline SDL_Window* GetWindow() {return m_window;}
      inline unsigned int GetWindowID() const {return SDL_GetWindowID(m_window);}

      void ResizeWindow(int w, int h);
      inline std::pair<int, int> GetSize() {return {m_width, m_height};}
   };
}
#endif /* end of include guard: DISPLAY_H */
