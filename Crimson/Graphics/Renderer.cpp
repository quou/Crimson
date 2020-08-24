#include "Renderer.h"

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <iostream>

void Renderer::Init() {
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
   SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
   SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
   SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
   SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
   SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
   SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

   if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
      std::cout << "Error loading OpenGL" << '\n';
   }

   std::cout << "OpenGL " << glGetString(GL_VERSION) << '\n';
   std::cout << glGetString(GL_RENDERER) << '\n';
}
