#include "Game.h"

#include "Logger.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "ImGuiImpl/ImGuiImpl.h"

namespace Crimson {
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}

	void Game::Run(const char* windowTitle, std::pair<int, int> windowSize) {
		CR_ASSERT(glfwInit(), "%s", "Unable to initialise GLFW");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 4);

		m_window = glfwCreateWindow(windowSize.first, windowSize.second, windowTitle, NULL, NULL);
		CR_ASSERT(m_window != NULL, "%s", "Unable to create window");

		glfwMakeContextCurrent(m_window);

		glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);

		CR_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "%s", "Unable to initialise OpenGL");

		CR_LOG("%s", "Initialisation successful");

		m_renderer = std::make_shared<Renderer>();

		ImGuiImpl::Init(m_window);

		OnInit();
		for (auto& l : m_layers) {
			l->OnInit();
		}

		while (!glfwWindowShouldClose(m_window)) {
			m_renderer->Clear();
			ImGuiImpl::BeginFrame();

			OnUpdate(1.0f);
			for (auto& l : m_layers) {
				l->OnUpdate(1.0f);
			}

			ImGuiImpl::EndFrame();

			glfwSwapBuffers(m_window);

			glfwPollEvents();
		}

		OnExit();

		for (auto& l : m_layers) {
			l.reset();
		}


		ImGuiImpl::Quit();

		glfwTerminate();
	}

	std::pair<int, int> Game::GetWindowSize() {
		std::pair<int, int> result;
		glfwGetWindowSize(m_window, &result.first, &result.second);
		return result;
	}
}
