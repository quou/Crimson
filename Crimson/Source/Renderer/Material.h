#pragma once

#include <memory>
#include <string>

#include "Shader.h"

struct lua_State;

namespace Crimson {
	class AssetManager;

	class Material {
	private:
		lua_State* L;
	public:
		std::shared_ptr<Shader> m_shader;

		Material(const std::string& config, AssetManager& assetManager);
		~Material();

		void Bind();
	};
}
