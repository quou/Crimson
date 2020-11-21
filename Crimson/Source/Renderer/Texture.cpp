#include "Texture.h"

#include <glad/glad.h>

#include "Logger.h"

namespace Crimson {

	Texture::Texture(Surface* surface) {
		if (!surface->pixels) {
			return;
		}

	   glGenTextures(1, &m_ID);
	   glBindTexture(GL_TEXTURE_2D, m_ID);

	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	   int mode = GL_RGB;

	   if(surface->componentCount == 4) {
	      mode = GL_RGBA;
	   }

	   m_width = surface->width;
	   m_height = surface->height;

	   glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->width, surface->height, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
	}

	Texture::~Texture() {
	   glDeleteTextures(1, &m_ID);
	}

	void Texture::Bind(unsigned int unit) const {
		// Maximum of 32 texture units
	   CR_ASSERT(unit < 31, "%s", "Texture unit too high");

	   glActiveTexture(GL_TEXTURE0 + unit);
	   glBindTexture(GL_TEXTURE_2D, m_ID);
	}
}
