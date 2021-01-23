#pragma once

#include "shader.h"
#include "memory.h"
#include "math/vec2.h"

namespace Crimson {
	class RenderTarget {
	private:
		unsigned int m_fb, m_rb;
		unsigned int m_output;

		vec2 m_size;
	public:
		RenderTarget(const vec2& size);
		~RenderTarget();

		void Resize(const vec2& size);

		void Bind();
		void Unbind();

		inline const vec2& GetSize() {return m_size;}
		inline unsigned int GetOutput() {return m_output;}
	};
}