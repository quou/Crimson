#include "Texture.h"

#include <iostream>
#include <assert.h>

#include <glad/glad.h>
#include "Utils/stb_image.h"

namespace Crimson {
   Texture::Texture(const std::string& fileName) {
      Load(fileName);
   }

   void Texture::Load(const std::string& fileName) {
      m_res = fileName;
      int width, height, componentCount;
      unsigned char* textureData = stbi_load(fileName.c_str(), &width, &height, &componentCount, 4);

      if (!textureData) {
         std::cerr << "Failed to load texture: " << fileName << '\n';
      }

      glGenTextures(1, &m_texture);
      glBindTexture(GL_TEXTURE_2D, m_texture);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

      stbi_image_free(textureData);
   }

   Texture::~Texture() {
      glDeleteTextures(1, &m_texture);
   }

   void Texture::Bind(unsigned int unit) {
      assert(unit >= 0 && unit <= 31);

      glActiveTexture(GL_TEXTURE0 + unit);
      glBindTexture(GL_TEXTURE_2D, m_texture);
   }
}
