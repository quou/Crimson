#pragma once

#include <memory>
#include <string>

#include "Shader.h"

struct lua_State;

namespace Crimson {
	class Material {
	private:
		lua_State* L;
	public:
		std::shared_ptr<Shader> m_shader;

		Material(const std::string& config, const std::string& shader);
		~Material();

		void Bind();
	};
}
