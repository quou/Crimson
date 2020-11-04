#pragma once

#include <tuple>

namespace Crimson {
	class RenderTarget {
	private:
		unsigned int m_fb, m_rb;
		unsigned int m_output;

		std::pair<int, int> m_size;
	public:
		RenderTarget(std::pair<int, int> size);
		~RenderTarget();

		void Resize(std::pair<int, int> size);

		void Bind();
		void Unbind();

		inline std::pair<int, int> GetSize() {return m_size;}
		inline unsigned int GetOutput() {return m_output;}
	};
}
