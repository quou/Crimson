#pragma once

#include <memory>
#include <string>

#include "Shader.h"
#include "Texture.h"

struct lua_State;

namespace Crimson {
	class AssetManager;

	class Material {
	private:
	public:
		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<Texture> m_albedo;

		Material(const std::string& config, AssetManager& assetManager);
		~Material();

		void Load(const std::string& config, AssetManager& assetManager);

		void SetFloat(const std::string& name, float value);
	   void SetInt(const std::string& name, int value);
	   void SetVec2(const std::string& name, glm::vec2 value);
	   void SetVec3(const std::string& name, glm::vec3 value);
	   void SetVec4(const std::string& name, glm::vec4 value);
	   void SetMat4(const std::string& name, glm::mat4 value);

		void Bind();
	};
}
