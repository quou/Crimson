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

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		Window* windowClass = (Window*)glfwGetWindowUserPointer(window);

		if (!windowClass) {
			Log(LogType::WARNING, "No window user pointer");
			return;
		}

		windowClass->m_eventSystem->KeyCallback(key, scancode, action, mods);
	}

	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		Window* windowClass = (Window*)glfwGetWindowUserPointer(window);

		if (!windowClass) {
			Log(LogType::WARNING, "No window user pointer");
			return;
		}

		windowClass->m_eventSystem->MouseButtonCallback(button, action, mods);
	}

	static void MouseMoveCallback(GLFWwindow* window, double x, double y) {
		Window* windowClass = (Window*)glfwGetWindowUserPointer(window);

		if (!windowClass) {
			Log(LogType::WARNING, "No window user pointer");
			return;
		}

		windowClass->m_eventSystem->MouseMoveCallback(x, y);
	}
	
	Window::Window(const char* title, int w, int h)
	: m_width(w), m_height(h) {
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

		m_window = glfwCreateWindow(w, h, title, NULL, NULL);
		if (!m_window) {
			Log(LogType::INFO, "failed to create window. Likely your GPU doesn't support OpenGL 4.2+");
			abort();
		}

		glfwMakeContextCurrent(m_window);
		glfwSwapInterval(0);

		m_eventSystem = ref<EventSystem>(new EventSystem());

		/* Needed by event callbacks to interface with the 
		 * window in a object-oriented manner */
		glfwSetWindowUserPointer(m_window, this);

		/* GLFW callbacks */
		glfwSetWindowSizeCallback(m_window, ResizeCallback);
		glfwSetKeyCallback(m_window, KeyCallback);
		glfwSetMouseButtonCallback(m_window, MouseButtonCallback);
		glfwSetCursorPosCallback(m_window, MouseMoveCallback);

		if (glfwRawMouseMotionSupported()) {
			glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		}

		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	}

	Window::~Window() {
		glfwTerminate();
	}

	void Window::Update() const {
		glfwSwapBuffers(m_window);
		
		m_eventSystem->Reset();
	}

	void Window::Poll() const {
		glfwPollEvents();
	}

	bool Window::ShouldClose() const {
		return glfwWindowShouldClose(m_window);
	}

	void Window::HideCursor() {
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void Window::ShowCursor() {
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void Window::SetCursorPos(const vec2& p) {
		glfwSetCursorPos(m_window, p.x, p.y);
	}
}
