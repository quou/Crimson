#include "Renderer.h"

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <iostream>

#include "Shader.h"
#include "SceneManagement/SceneManager.h"

namespace Crimson {

   void Renderer::Init() {
      if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
         std::cout << "Error loading OpenGL" << '\n';
      }
      glViewport(0, 0, 1366, 768);

      glEnable(GL_DEPTH_TEST);
      glEnable(GL_CULL_FACE);
      glCullFace(GL_BACK);

      glEnable(GL_MULTISAMPLE);

      std::cout << "OpenGL " << glGetString(GL_VERSION) << '\n';
      std::cout << glGetString(GL_RENDERER) << '\n';
   }

   void Renderer::Clear(float r, float g, float b, float a) {
      glClearColor(r, g, b, a);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   }

   void Renderer::AddShader(Shader* shader) {
      m_shaders.push_back(shader);
   }


   /*void Renderer::Draw(unsigned int vertexArray, unsigned int drawCount, unsigned int shader) {
      glUseProgram(shader);
      glBindVertexArray(vertexArray);

      glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);

      glBindVertexArray(0);
      glUseProgram(0);
   }*/

}
