#include <GLFW/glfw3.h>

#include "application.h"
#include "rendering/renderer.h"
#include "assets.h"
#include "logger.h"

namespace Crimson {
	void Application::Run(const char* windowTitle, int w, int h) {
		InitLogger();
		
		m_window = ref<Window>(new Window(windowTitle, w, h));

		AssetManager::Init("res");
		Renderer::Init(m_window);

		OnInit();

		/* For calculating delta time */
		float last, now, delta;

		while (!m_window->ShouldClose()) {
			/* Calculate delta time */
			now = glfwGetTime();
			delta = now - last;
			last = now;

			OnUpdate(delta);

			m_window->Update();
		}

		AssetManager::Quit();
		CloseLogger();
	}
}
