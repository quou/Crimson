#include "Cubemap.h"

#include <glad/glad.h>

#include "Utils/stb_image.h"

#include "Camera.h"

#include <iostream>

namespace Crimson {
   Cubemap::Cubemap() { }

   Cubemap::Cubemap(std::vector<std::string> facePaths, const std::string vertShader, const std::string& fragShader) {
      Load(facePaths, vertShader, fragShader);
   }

   void Cubemap::Load(std::vector<std::string> facePaths, const std::string vertShader, const std::string& fragShader) {
      m_skyShader.Init(vertShader, fragShader);

      // textures
      glGenTextures(1, &m_textureID);
      glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

      for (size_t i = 0; i < 6; i++) {
         int width, height, componentCount;
         unsigned char* textureData = stbi_load(facePaths[i].c_str(), &width, &height, &componentCount, 4);
         if (!textureData) {
            std::cerr << "Failed to load texture: " << facePaths[i] << '\n';
            return;
         }

         glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
         stbi_image_free(textureData);
      }

      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      // Mesh
      unsigned int skyboxIndices[] = {
         // front
         0, 1, 2,
         2, 1, 3,
         // right
         2, 3, 5,
         5, 3, 7,
         // back
         5, 7, 4,
         4, 7, 6,
         // left
         4, 6, 0,
         0, 6, 1,
         // top
         4, 0, 5,
         5, 0, 2,
         // bottom
         1, 6, 3,
         3, 6, 7
      };

      Vertex skyboxVertices[] = {
         {glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(0,0), glm::vec3(0,0,0)},
         {glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0,0), glm::vec3(0,0,0)},
         {glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(0,0), glm::vec3(0,0,0)},
         {glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(0,0), glm::vec3(0,0,0)},
         {glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0,0), glm::vec3(0,0,0)},
         {glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0,0), glm::vec3(0,0,0)},
         {glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0,0), glm::vec3(0,0,0)},
         {glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(0,0), glm::vec3(0,0,0)},
      };

      m_skyModel.LoadFromData(skyboxVertices, 64, skyboxIndices, 36);
   }

   void Cubemap::Render(Camera& camera) {
      glm::mat3 view = glm::mat3(camera.GetView());

      glDepthMask(GL_FALSE);

      m_skyShader.Bind();
      m_skyShader.SetUniformMatrix4("view", camera.GetProjection() * glm::mat4(view));
      m_skyShader.SetUniform1i("skybox", 0);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

      m_skyModel.Render();

      glDepthMask(GL_TRUE);
   }
}
