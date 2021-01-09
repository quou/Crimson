#include "application.h"
#include "rendering/renderer.h"
#include "assets.h"
#include "logger.h"

namespace Crimson {
	void Application::Run(const char* windowTitle, int w, int h) {
		InitLogger();
		
		m_window = ref<Window>(new Window(windowTitle, w, h));

		AssetManager::Init("res");
		Renderer::Init();

		OnInit();

		while (!m_window->ShouldClose()) {
			OnUpdate(1.0f);

			m_window->Update();
		}

		AssetManager::Quit();
		CloseLogger();
	}
}
