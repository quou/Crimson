#include "Input.h"

#include "GLFW/glfw3.h"

#include "Logger.h"

extern "C" {
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

namespace Crimson {

	void Input::Init() {
		RegisterKey("jump", CR_KEY_SPACE);
		RegisterKey("up", CR_KEY_UP);
		RegisterKey("down", CR_KEY_DOWN);
		RegisterKey("left", CR_KEY_LEFT);
		RegisterKey("right", CR_KEY_RIGHT);
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
		lua_State* L = luaL_newstate();

		int r = luaL_dostring(L, lua);
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
}
