#include "Game.h"

#include "Logger.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "ImGuiImpl/ImGuiImpl.h"
#include "Renderer/Renderer.h"

#include "Input.h"

namespace Crimson {
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		Input::IKeyCallback(key, scancode, action, mods);
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
		glfwSetKeyCallback(m_window, KeyCallback);

		CR_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "%s", "Unable to initialise OpenGL");

		CR_LOG("%s", "Initialisation successful");

		Renderer::Init();

		ImGuiImpl::Init(m_window);

		OnInit();
		for (auto& l : m_layers) {
			l->OnInit();
		}

		float oldTimeSinceStart;

		while (!glfwWindowShouldClose(m_window)) {
			float timeSinceStart = glfwGetTime();
			float delta = timeSinceStart - oldTimeSinceStart;
			oldTimeSinceStart = timeSinceStart;

			Renderer::Clear();
			ImGuiImpl::BeginFrame();

			OnUpdate(delta);
			for (auto& l : m_layers) {
				l->OnUpdate(delta);
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
