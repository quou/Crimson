#include <string.h>

#include <GLFW/glfw3.h>

#include "input.h"

namespace Crimson {
	EventSystem::EventSystem() {
		memset(m_inputs, 0, CR_KEY_COUNT * sizeof(bool));
		memset(m_downKeys, 0, CR_KEY_COUNT * sizeof(bool));
		memset(m_upKeys, 0, CR_KEY_COUNT * sizeof(bool));

		memset(m_mouseInput, 0, CR_MOUSE_BUTTON_COUNT * sizeof(bool));
		memset(m_downMouse, 0, CR_MOUSE_BUTTON_COUNT * sizeof(bool));
		memset(m_upMouse, 0, CR_MOUSE_BUTTON_COUNT * sizeof(bool));
	}

	EventSystem::~EventSystem() {

	}

	void EventSystem::Reset() {
		memset(m_downKeys, 0, CR_KEY_COUNT * sizeof(bool));
		memset(m_upKeys, 0, CR_KEY_COUNT * sizeof(bool));

		memset(m_downMouse, 0, CR_MOUSE_BUTTON_COUNT * sizeof(bool));
		memset(m_upMouse, 0, CR_MOUSE_BUTTON_COUNT * sizeof(bool));
	}

	void EventSystem::KeyCallback(int key, int scancode, int action, int mods) {
		m_downKeys[key] = action == GLFW_PRESS;
		m_upKeys[key] = action == GLFW_RELEASE;
		m_inputs[key] = action == GLFW_PRESS;
	}

	void EventSystem::MouseButtonCallback(int button, int action, int mods) {
		m_downMouse[button] = action == GLFW_PRESS;
		m_upMouse[button] = action == GLFW_RELEASE;
		m_mouseInput[button] = action == GLFW_PRESS;
	}

	void EventSystem::MouseMoveCallback(float x, float y) {
		m_mousePosition = vec2(x, y);
	}
}