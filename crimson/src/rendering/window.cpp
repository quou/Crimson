#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "window.h"
#include "logger.h"
#include "renderer.h"

namespace Crimson {
	void ResizeCallback(GLFWwindow* window, int w, int h) {
		Window* windowClass = (Window*)glfwGetWindowUserPointer(window);

		if (!windowClass) {
			Log(LogType::WARNING, "No window user pointer");
			return;
		}

		windowClass->m_width = w;
		windowClass->m_height = h;
	}
	
	Window::Window(const char* title, int w, int h) 
	: m_width(w), m_height(h) {
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow(w, h, title, NULL, NULL);
		glfwMakeContextCurrent(m_window);

		/* Needed by ResizeCallback to set the window's width and height
		 * member variables */
		glfwSetWindowUserPointer(m_window, this);

		/* GLFW callbacks */
		glfwSetWindowSizeCallback(m_window, ResizeCallback);

		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	}

	Window::~Window() {
		glfwTerminate();
	}

	void Window::Update() const {
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	bool Window::ShouldClose() const {
		return glfwWindowShouldClose(m_window);
	}
}
