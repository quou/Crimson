#include "ProjectConfig.h"

extern "C" {
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

#include "Logger.h"

namespace Crimson {
	bool CheckLua(lua_State* L, int r) {
		if (r != LUA_OK) {
			CR_LOG_ERROR("Error loading material: %s", lua_tostring(L, -1));
			return false;
		}
		return true;
	}

	ProjectConfig LoadProjectConfig(const std::string& lua) {
		auto result = ProjectConfig{"nil"};

		lua_State* L;

		L = luaL_newstate();

		if (!CheckLua(L, luaL_dostring(L, lua.c_str()))) {
			goto end;
		}

		lua_getglobal(L, "project");
		if (lua_istable(L, -1)) {
			lua_pushstring(L, "startup");
			lua_gettable(L, -2);
			if (lua_isstring(L, -1)) {
				result.startup = lua_tostring(L, -1);
			}

			lua_pop(L, 1);
		}

		end:
			lua_close(L);

		return result;
	}
}
