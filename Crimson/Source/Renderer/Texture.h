#pragma once

#include <tuple>

#include "Surface.h"

namespace Crimson {

	class Texture {
	private:
	   unsigned int m_ID;
	   unsigned int m_width, m_height;
	public:
	   Texture(Surface* surface);
	   ~Texture();

	   inline std::pair<int, int> GetSize() const {
	      return {m_width, m_height};
	   }

		inline unsigned int GetID() const {return m_ID;}

	   void Bind(unsigned int unit) const;
	};

}
