#pragma once

#include "rendering/window.h"
#include "memory.h"
#include "core.h"
#include "scripting/scriptmanager.h"

namespace Crimson {
	class CR_API ImGuiManager {
	private:
		ref<ScriptManager> m_scriptManager;
		bool m_useScript = false;

		static ImGuiManager& instance() {
			static ImGuiManager i;
			return i;
		}
	public:
		static void Init(const ref<Window>& window);
		static void Quit();
		static void BeginFrame();
		static void EndFrame();

		static void UseScript(const char* scriptPath);
	};
}
