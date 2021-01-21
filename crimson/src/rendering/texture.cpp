#include <assert.h>

#include <glad/glad.h>

#include "texture.h"
#include "logger.h"
#include "utils/stb_image.h"

namespace Crimson {
	Texture::Texture(unsigned char* pixels, int w, int h, int components)
       		: m_width(w), m_height(h), m_components(components) {
		if (pixels == NULL) {
			Log(LogType::ERROR, "Cannot construct texture with null pixel data");
			return;
		}

		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int mode = GL_RGB;
		if (m_components == 4) {
			mode = GL_RGBA;
		}
		
		glTexImage2D(GL_TEXTURE_2D, 0, mode, m_width, m_height, 
				0, mode, GL_UNSIGNED_BYTE, pixels);

		stbi_image_free(pixels);

		m_error = false;
	}

	Texture::~Texture() {
		glDeleteTextures(1, &m_id);
	}

	void Texture::Bind(unsigned int unit) {
		if (m_error) { return; }

		assert(unit <= 32 && "Texture unit too high");

		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	void Texture::Unbind() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
