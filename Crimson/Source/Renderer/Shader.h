#pragma once

#include <glm/glm.hpp>

#include <string>

namespace Crimson {

	class Shader {
	private:
	   unsigned int m_ID;
	public:
	   Shader(const std::string& code);

	   void Init(const std::string& code);

	   Shader& Bind();

	   void SetFloat(const std::string& name, float value);
	   void SetInt(const std::string& name, int value);
	   void SetVec2(const std::string& name, glm::vec2 value);
	   void SetVec3(const std::string& name, glm::vec3 value);
	   void SetVec4(const std::string& name, glm::vec4 value);
	   void SetMat4(const std::string& name, glm::mat4 value);

	   ~Shader();
	};

}
