#include "Material.h"

#include <string>
#include <vector>
#include <tuple>
#include <sstream>

extern "C" {
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

#include "Logger.h"

bool CheckLua(lua_State* L, int r) {
	if (r != LUA_OK) {
		CR_LOG_ERROR("%s", lua_tostring(L, -1));
		return false;
	}
	return true;
}

static std::vector<std::pair<std::string, std::string>> ParseUniforms(const std::string& shader) {
	std::vector<std::pair<std::string, std::string>> result;

	std::stringstream ss(shader);
	std::string line;

	while (std::getline(ss, line)) {
		std::vector<std::string> lineSplit;
		std::string currentSub;
		for (char c : line) {
			if (c == ' ' || c == ';') {
				if (!currentSub.empty()) {
					lineSplit.push_back(currentSub);
				}
				currentSub = "";
			} else {
				currentSub += c;
			}
		}

		if (lineSplit.size() > 0) {
			if (lineSplit[0] == "uniform") {
				result.push_back({lineSplit[1], lineSplit[2]});
			}
		}
	}

	return result;
}

namespace Crimson {
	Material::Material(const std::string& config, const std::string& shader) {
		m_shader = std::make_shared<Shader>(shader);

		L = luaL_newstate();

		if (!CheckLua(L, luaL_dostring(L, config.c_str()))) {
			return;
		}

		m_shader->Bind();

		lua_getglobal(L, "material");
		if (!lua_istable(L, -1)) {return;};

		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
			CR_PRINTF("%s, %s\n",
				lua_tostring(L, -2),
				lua_typename(L, lua_type(L, -1)));

			if (lua_isnumber(L, -1)) {
				m_shader->SetFloat("u_" + std::string(lua_tostring(L, -2)), lua_tonumber(L, -1));
			} else if (lua_istable(L, -1)) {
				int size = lua_rawlen(L, -1);
				CR_PRINTF("%d\n", size);

				if (size == 2) {
					lua_pushnil(L);
					glm::vec2 v;

					lua_rawgeti(L, -1, 1);
					if (lua_isnumber(L, -1)) {
						v.x = lua_tonumber(L, -1);
					}
					lua_pop(L, 1);

					lua_rawgeti(L, -1, 2);
					if (lua_isnumber(L, -1)) {
						v.y = lua_tonumber(L, -1);
					}
					lua_pop(L, 1);

					lua_pop(L, 1);

					m_shader->SetVec2("u_" + std::string(lua_tostring(L, -2)), v);
				} else if (size == 3) {
					lua_pushnil(L);
					glm::vec3 v;

					lua_rawgeti(L, -1, 1);
					if (lua_isnumber(L, -1)) {
						v.x = lua_tonumber(L, -1);
					}
					lua_pop(L, 1);

					lua_rawgeti(L, -1, 2);
					if (lua_isnumber(L, -1)) {
						v.y = lua_tonumber(L, -1);
					}
					lua_pop(L, 1);

					lua_rawgeti(L, -1, 3);
					if (lua_isnumber(L, -1)) {
						v.z = lua_tonumber(L, -1);
					}
					lua_pop(L, 1);

					lua_pop(L, 1);

					m_shader->SetVec3("u_" + std::string(lua_tostring(L, -2)), v);
				} else if (size == 4) {
					lua_pushnil(L);
					glm::vec4 v;

					lua_rawgeti(L, -1, 1);
					if (lua_isnumber(L, -1)) {
						v.x = lua_tonumber(L, -1);
					}
					lua_pop(L, 1);

					lua_rawgeti(L, -1, 2);
					if (lua_isnumber(L, -1)) {
						v.y = lua_tonumber(L, -1);
					}
					lua_pop(L, 1);

					lua_rawgeti(L, -1, 3);
					if (lua_isnumber(L, -1)) {
						v.z = lua_tonumber(L, -1);
					}
					lua_pop(L, 1);

					lua_rawgeti(L, -1, 3);
					if (lua_isnumber(L, -1)) {
						v.w = lua_tonumber(L, -1);
					}
					lua_pop(L, 1);

					lua_pop(L, 1);

					m_shader->SetVec3("u_" + std::string(lua_tostring(L, -2)), v);
				} else if (size > 4) {
					CR_LOG_ERROR("%s", "Vectors of size 5+ are not supported");
				} else if (size == 1) {
					CR_LOG_ERROR("%s", "Vectors of size 1 are not supported, use number instead");
				}
			} else {
				CR_LOG_ERROR("Type \"%s\" isn't supported for materials", lua_typename(L, lua_type(L, -1)));
			}

			lua_pop(L, 1);
		}
	}

	Material::~Material() {
		lua_close(L);
	}
}
