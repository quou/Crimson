#include <GLFW/glfw3.h>

#include "application.h"
#include "rendering/renderer.h"
#include "assets.h"
#include "logger.h"
#include "imgui/imguimanager.h"

namespace Crimson {
	void Application::Run(const char* windowTitle, int w, int h) {
		InitLogger();
		
		m_window = ref<Window>(new Window(windowTitle, w, h));

		AssetManager::Init("res");
		Renderer::Init(m_window);

		OnInit();

		/* For calculating delta time */
		float last = 0.0f, now = 0.0f, delta = 0.0f;

		while (!m_window->ShouldClose()) {
			m_window->Poll();

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
