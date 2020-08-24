#include "Renderer.h"

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <iostream>

void Renderer::Init() {
   if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
      std::cout << "Error loading OpenGL" << '\n';
   }

   std::cout << "OpenGL " << glGetString(GL_VERSION) << '\n';
   std::cout << glGetString(GL_RENDERER) << '\n';
}

void Renderer::Clear(float r, float g, float b, float a) {
   glClearColor(r, g, b, a);
   glClear(GL_COLOR_BUFFER_BIT);
}
