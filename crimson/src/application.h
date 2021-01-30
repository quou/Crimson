#pragma once

#include "memory.h"
#include "rendering/window.h"
#include "core.h"

namespace Crimson {
	class CR_API Application {
	protected:
		virtual void OnInit() {}
		virtual void OnUpdate(float delta) {}
	public:
		ref<Window> m_window;

		void Run(const char* windowTitle, int w, int h);
	
		virtual ~Application() {}
	};
}
