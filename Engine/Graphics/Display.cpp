#include "Display.h"

#include <glad/glad.h>

#include <iostream>

Display::Display(int width, int height, int flags, const char* title) {

   m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | flags);
   m_glContext = SDL_GL_CreateContext(m_window);

   /* TODO: OpenGL initialisation should be inside a renderer class */
   if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
      std::cout << "Error loading OpenGL" << '\n';
   }

   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
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
