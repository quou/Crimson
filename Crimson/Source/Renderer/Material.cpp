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
#include "AssetManager.h"

bool CheckLua(lua_State* L, int r) {
	if (r != LUA_OK) {
		CR_LOG_ERROR("%s", lua_tostring(L, -1));
		return false;
	}
	return true;
}

namespace Crimson {
	Material::Material(const std::string& config, AssetManager& assetManager) {

		L = luaL_newstate();

		if (!CheckLua(L, luaL_dostring(L, config.c_str()))) {
			return;
		}

		lua_getglobal(L, "shader");
		if (lua_isstring(L, -1)) {
			m_shader = std::make_shared<Shader>(assetManager.LoadText(lua_tostring(L, -1)));
		} else {
			return;
		}
		lua_pop(L, 1);

		lua_getglobal(L, "albedo");
		if (lua_isstring(L, -1)) {
			m_albedo = std::make_shared<Texture>(assetManager.LoadSurface(lua_tostring(L, -1)));
		} else {
			return;
		}
		lua_pop(L, 1);

		m_shader->Bind();

		lua_getglobal(L, "material");
		if (!lua_istable(L, -1)) {return;};

		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
			if (lua_isnumber(L, -1)) {
				m_shader->SetFloat("u_material." + std::string(lua_tostring(L, -2)), lua_tonumber(L, -1));
			} else if (lua_istable(L, -1)) {
				int size = lua_rawlen(L, -1);

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

					m_shader->SetVec2("u_material." + std::string(lua_tostring(L, -2)), v);
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

					m_shader->SetVec3("u_material." + std::string(lua_tostring(L, -2)), v);
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

					m_shader->SetVec3("u_material." + std::string(lua_tostring(L, -2)), v);
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
