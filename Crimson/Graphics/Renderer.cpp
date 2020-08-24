#include "Renderer.h"

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <iostream>

namespace Crimson {

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

   void Renderer::Draw(unsigned int vertexArray, unsigned int drawCount, unsigned int shader) {
      glUseProgram(shader);
      glBindVertexArray(vertexArray);

      glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);

      glBindVertexArray(0);
      glUseProgram(0);
   }

}
