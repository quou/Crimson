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

	EventSystem& EventSystem::instance() {
		static EventSystem e;
		return e;
	}

	void EventSystem::Reset() {
		memset(instance().m_downKeys, 0, CR_KEY_COUNT * sizeof(bool));
		memset(instance().m_upKeys, 0, CR_KEY_COUNT * sizeof(bool));

		memset(instance().m_downMouse, 0, CR_MOUSE_BUTTON_COUNT * sizeof(bool));
		memset(instance().m_upMouse, 0, CR_MOUSE_BUTTON_COUNT * sizeof(bool));
	}

	void EventSystem::KeyCallback(int key, int scancode, int action, int mods) {
		instance().m_downKeys[key] = action == GLFW_PRESS;
		instance().m_upKeys[key] = action == GLFW_RELEASE;
		instance().m_inputs[key] = action == GLFW_PRESS || action == GLFW_REPEAT;
	}

	void EventSystem::MouseButtonCallback(int button, int action, int mods) {
		instance().m_downMouse[button] = action == GLFW_PRESS;
		instance().m_upMouse[button] = action == GLFW_RELEASE;
		instance().m_mouseInput[button] = action == GLFW_PRESS || action == GLFW_REPEAT;
	}

	void EventSystem::MouseMoveCallback(float x, float y) {
		instance().m_mousePosition = vec2(x, y);
	}
}