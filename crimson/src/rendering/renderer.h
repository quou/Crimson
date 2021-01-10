#pragma once

#include "core.h"
#include "memory.h"
#include "window.h"

namespace Crimson {
	class CR_API Renderer {
	private:
		ref<Window> m_window;
	public:
		inline static Renderer& instance() {
			static Renderer r;
			return r;
		}

		static void Init(const ref<Window>& window);
	
		static void Clear(float r, float g, float b);
	};
}
