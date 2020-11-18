#include "Input.h"

#include "GLFW/glfw3.h"

#include "Logger.h"

extern "C" {
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

#include <fstream>
#include <sstream>
#include <algorithm>

namespace Crimson {

	void Input::Init() {
		RegisterKey("jump", CR_KEY_SPACE);
		RegisterKey("up", CR_KEY_UP);
		RegisterKey("down", CR_KEY_DOWN);
		RegisterKey("left", CR_KEY_LEFT);
		RegisterKey("right", CR_KEY_RIGHT);
		RegisterKey("w", CR_KEY_W);
		RegisterKey("a", CR_KEY_A);
		RegisterKey("s", CR_KEY_S);
		RegisterKey("d", CR_KEY_D);
		RegisterKey("left_mouse_button", CR_MOUSE_BUTTON_1);
		RegisterKey("right_mouse_button", CR_MOUSE_BUTTON_2);
		RegisterKey("middle_mouse_button", CR_MOUSE_BUTTON_3);
	}

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

	const Key& Input::GetKey(std::string name) {
		if (instance().m_keys.count(name) == 0) {
			CR_LOG_ERROR("Button '%s' not found in input config", name.c_str());
		}

		return instance().m_keys[name];
	}

	void Input::LoadConfig(const char* lua) {
		std::string str = lua;

		lua_State* L = luaL_newstate();

		int r = luaL_dostring(L, str.c_str());
		if (r != LUA_OK) {
			CR_LOG_ERROR("Error loading input config: %s", lua_tostring(L, -1));
			goto end;
		}

		lua_getglobal(L, "input");
		if (!lua_istable(L, -1)) {
			CR_LOG_ERROR("%s", "No input config table found");
			goto end;
		}

		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
			if (lua_isnumber (L, -1)) {
				RegisterKey(lua_tostring(L, -2), lua_tointeger(L, -1));
			}

			lua_pop(L, 1);
		}

		end:
			lua_close(L);
	}

	void Input::SaveConfig(const char* filePath) {
		std::ofstream file(filePath);

		if (file.good()) {
			std::stringstream ss;

			ss << "input = {\n";
			for (auto& key : instance().m_keys) {
				ss << "\t" << key.first << " = " << key.second.keycode << ",\n";
			}
			ss << "}";

			std::string str = ss.str();

			file << str;
		}
	}

	void Input::IMouseButtonCallback(int button, int action, int mods) {
		for (auto& x : instance().m_keys) {
			if (x.second.keycode == button) {
				x.second.action = action;
				if (action == GLFW_PRESS) {
					x.second.pressed = true;
				} else if (action == GLFW_RELEASE) {
					x.second.pressed = false;
				}
			}
		}
	}

	void Input::IMouseMovementCallback(double x, double y) {
		instance().mousePos = glm::vec2(x, y);

		auto& in = instance();

		if (in.firstInput) {
			in.firstInput = false;
			in.oldMousePos = glm::vec2(x, y);
		}

		float changeX = x - instance().oldMousePos.x;
		float changeY = instance().oldMousePos.y - y;

		in.mouseChange = glm::vec2(changeX, changeY);

		in.oldMousePos = glm::vec2(x, y);
	}

	void Input::IScrollCallback(double x, double y) {
		instance().mouseDelta = glm::vec2(x, y);
	}

	void Input::IEndFrame() {
		instance().mouseChange = glm::vec2(0);
		instance().mouseDelta = glm::vec2(0);
	}

	glm::vec2 Input::GetMouseChange() {
		return instance().mouseChange;
	}

	glm::vec2 Input::GetScrollDelta() {
		return instance().mouseDelta;
	}

	void Input::SetCursorPosition(GLFWwindow* window, glm::vec2 newPos) {
		instance().mouseChange = glm::vec2(0, 0);
		instance().oldMousePos = newPos;

		glfwSetCursorPos(window, newPos.x, newPos.y);
	}
}
