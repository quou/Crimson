#pragma once

#include "core.h"

namespace Crimson {
	class CR_API Texture {
	private:
		unsigned int m_id;

		int m_width, m_height, m_components;
	public:
		Texture(unsigned char* pixels, int w, int h, int components);
		virtual ~Texture();

		void Bind(unsigned int unit);
		void Unbind();

		inline int GetWidth() const { return m_width; }
		inline int GetHeight() const { return m_height; }
		inline int GetComponentCount() const { return m_components; }
	};
}
