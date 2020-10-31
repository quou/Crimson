#include "Input.h"

#include "GLFW/glfw3.h"

namespace Crimson {

	void Input::IKeyCallback(int key, int scancode, int action, int mods) {
		for (auto& x : instance().m_keys) {
			if (x.second.keycode == key) {
				x.second.action = action;
				if (action == GLFW_PRESS) {
					x.second.pressed = true;
				} else if (action == GLFW_RELEASE) {
					x.second.pressed = false;
				}
			}
		}
	}

	void Input::RegisterKey(std::string name, int keycode) {
		instance().m_keys[name] = Key{keycode, 0, false};
	}

	const Key Input::GetKey(std::string name) {
		return instance().m_keys[name];
	}

}
