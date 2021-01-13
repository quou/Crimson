#pragma once

#include "memory.h"
#include "rendering/window.h"
#include "core.h"

namespace Crimson {
	class CR_API Application {
	protected:
		ref<Window> m_window;
		
		virtual void OnInit() {}
		virtual void OnUpdate(float delta) {}
	public:
		void Run(const char* windowTitle, int w, int h);
	
		virtual ~Application() {}
	};
}