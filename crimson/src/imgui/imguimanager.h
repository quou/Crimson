#pragma once

#include "rendering/window.h"
#include "memory.h"

namespace Crimson {
	class ImGuiManager {
	public:
		static void Init(const ref<Window>& window);
		static void Quit();
		static void BeginFrame();
		static void EndFrame();
	};
}
