#include "Game.h"

#include "Logger.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "ImGuiImpl/ImGuiImpl.h"

namespace Crimson {
	void Game::Run(const char* windowTitle, std::pair<int, int> windowSize) {
		CR_ASSERT(glfwInit(), "%s\n", "Unable to initialise GLFW");

		m_window = glfwCreateWindow(windowSize.first, windowSize.second, windowTitle, NULL, NULL);
		CR_ASSERT(m_window != NULL, "%s\n", "Unable to create window");

		glfwMakeContextCurrent(m_window);

		CR_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "%s\n", "Unable to initialise OpenGL");

		CR_LOG("%s\n", "Initialisation successful");
		CR_LOG("Using OpenGL version %s\n", glGetString(GL_VERSION));
		CR_LOG("Renderer: %s\n", glGetString( GL_RENDERER));

		// glEnable(GL_DEBUG_OUTPUT);
		// glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		// glDebugMessageCallback(glDebugOutput, nullptr);
		// glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

		ImGuiImpl::Init(m_window);

		OnInit();
		for (auto& l : m_layers) {
			l->OnInit();
		}

		while (!glfwWindowShouldClose(m_window)) {
			glClear(GL_COLOR_BUFFER_BIT);
			ImGuiImpl::BeginFrame();

			OnUpdate(1.0f);
			for (auto& l : m_layers) {
				l->OnUpdate(1.0f);
			}

			ImGuiImpl::EndFrame();

			glfwSwapBuffers(m_window);

			glfwPollEvents();
		}

		for (auto& l : m_layers) {
			l.reset();
		}

		ImGuiImpl::Quit();

		glfwTerminate();
	}
}
