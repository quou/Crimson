#pragma once

#include "memory.h"
#include "window.h"
#include "core.h"

namespace Crimson {
	class CR_API Application {
	private:
		ref<Window> m_window;
	protected:
		virtual void OnInit() {}
		virtual void OnUpdate(float delta) {}
	public:
		void Run(const char* windowTitle, int w, int h);
	
		virtual ~Application() {}
	};
}
