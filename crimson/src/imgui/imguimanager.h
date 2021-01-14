#pragma once

#include "rendering/window.h"
#include "memory.h"
#include "core.h"

namespace Crimson {
	class CR_API ImGuiManager {
	private:
		static ImGuiManager& instance() {
			static ImGuiManager i;
			return i;
		}
	public:
		static void Init(const ref<Window>& window);
		static void Quit();
		static void BeginFrame();
		static void EndFrame();
	};
}
