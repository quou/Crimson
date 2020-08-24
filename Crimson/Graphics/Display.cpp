#include "Display.h"

#include <glad/glad.h>

#include <iostream>

namespace Crimson {

   void Display::Init(int width, int height, int flags, const char* title) {

      m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | flags);
      m_glContext = SDL_GL_CreateContext(m_window);
   }

   void Display::Clear(float r, float g, float b, float a) {
      glClearColor(r, g, b, a);
      glClear(GL_COLOR_BUFFER_BIT);
   }

   void Display::Present() {
      SDL_GL_SwapWindow(m_window);
   }

   Display::~Display() {
      SDL_GL_DeleteContext(m_glContext);
      SDL_DestroyWindow(m_window);
   }
}
