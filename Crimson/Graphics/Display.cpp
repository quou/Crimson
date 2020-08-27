#include "Display.h"

#include <glad/glad.h>

#include <iostream>

namespace Crimson {

   void Display::Init(int width, int height, int flags, const char* title) {
      m_width = width;
      m_height = height;

      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
      SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
      SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
      SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
      SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
      SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
      SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
      SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

      m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | flags);
      m_glContext = SDL_GL_CreateContext(m_window);
      SDL_GL_MakeCurrent(m_window, m_glContext);
   }

   void Display::Present() {
      SDL_GL_SwapWindow(m_window);
   }

   Display::~Display() {
      SDL_GL_DeleteContext(m_glContext);
      SDL_DestroyWindow(m_window);
   }

   void Display::ResizeWindow(int w, int h) {
      m_width = w;
      m_height = h;
      glViewport(0,0,w,h);
   }
}
