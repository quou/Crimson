#include "Game.h"

#include "Logger.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "ImGuiImpl/ImGuiImpl.h"
#include "Renderer/Renderer.h"

#include "CrimsonIcon.h"

#include "Input.h"

namespace Crimson {
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		Input::IKeyCallback(key, scancode, action, mods);
	}

	static void MouseCallback(GLFWwindow* window, double x, double y) {
		Input::IMouseMovementCallback(x, y);
	}

	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		Input::IMouseButtonCallback(button, action, mods);
	}

	static void MouseScrollCallback(GLFWwindow* window, double x, double y) {
		Input::IScrollCallback(x, y);
	}

	void Game::SetCursorPosition(glm::vec2 newPos) {
		Input::SetCursorPosition(m_window, newPos);
	}

	void Game::Run(const char* windowTitle, std::pair<int, int> windowSize) {
		CR_ASSERT(glfwInit(), "%s", "Unable to initialise GLFW");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

		m_window = glfwCreateWindow(windowSize.first, windowSize.second, windowTitle, NULL, NULL);
		CR_ASSERT(m_window != NULL, "%s", "Unable to create window");

		GLFWimage iconImage = {75, 75, CrimsonIconImage};
		glfwSetWindowIcon(m_window, 1, &iconImage);

		glfwMakeContextCurrent(m_window);
		glfwSwapInterval(true);

		glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);
		glfwSetKeyCallback(m_window, KeyCallback);
		glfwSetMouseButtonCallback(m_window, MouseButtonCallback);
		glfwSetCursorPosCallback(m_window, MouseCallback);
		glfwSetScrollCallback(m_window, MouseScrollCallback);

		CR_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "%s", "Unable to initialise OpenGL");

		CR_LOG("%s", "Initialisation successful");

		Renderer::Init();

		ImGuiImpl::Init(m_window);

		OnInit();
		for (auto& l : m_layers) {
			l->m_game = this;
			l->OnInit();
		}

		float delta = 0.0f;
		float last = 0.0f;

		while (true) {
			if (glfwWindowShouldClose(m_window)) {
				m_shouldExit = true;
				for (auto& l : m_layers) {
					l->OnExit();
				}
			}

			float now = (float)glfwGetTime();

			Renderer::Clear();
			ImGuiImpl::BeginFrame();

			OnUpdate(delta);
			for (auto& l : m_layers) {
				l->OnUpdate(delta);
			}

			Renderer::EndFrame();

			Input::IEndFrame();
			ImGuiImpl::EndFrame();

			glfwSwapBuffers(m_window);

			glfwPollEvents();

			delta = (float)glfwGetTime() - now;

			if (m_shouldExit) {
				break;
			}
		}

		OnExit();

		for (auto& l : m_layers) {
			l.reset();
		}

		Renderer::DeInit();

		ImGuiImpl::Quit();

		glfwTerminate();
	}

	void Game::CancelExit() {
		glfwSetWindowShouldClose(m_window, GLFW_FALSE);
		m_shouldExit = false;
	}

	void Game::Exit() {
		glfwSetWindowShouldClose(m_window, GLFW_TRUE);
		m_shouldExit = true;
	}

	std::pair<int, int> Game::GetWindowSize() {
		std::pair<int, int> result;
		glfwGetWindowSize(m_window, &result.first, &result.second);
		return result;
	}
}
