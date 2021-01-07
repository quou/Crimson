#include <GLFW/glfw3.h>

#include "window.h"
#include "logger.h"
#include "renderer.h"

namespace Crimson {
	Window::Window(const char* title, int w, int h) 
	: m_width(w), m_height(h) {
		glfwInit();

		m_window = glfwCreateWindow(w, h, title, NULL, NULL);
		glfwMakeContextCurrent(m_window);
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
