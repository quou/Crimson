#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "window.h"
#include "logger.h"
#include "renderer.h"

namespace Crimson {
	Window::Window(const char* title, int w, int h) 
	: m_width(w), m_height(h) {
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow(w, h, title, NULL, NULL);
		glfwMakeContextCurrent(m_window);

		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	}

	Window::~Window() {
		glfwTerminate();
	}

	void Window::Update() {
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	bool Window::ShouldClose() {
		return glfwWindowShouldClose(m_window);
	}
}
