#pragma once

#include <vector>

#include "core.h"

namespace Crimson {
	struct vec2;
	struct vec3;
	struct vec4;
	struct mat4;

	/* A generic GLSL shader class */
	class CR_API Shader {
	private:
		/* OpenGL ID */
		unsigned int m_id;
		
		/* To avoid spitting out errors every frame */
		bool m_panicMode;
	public:
		Shader(const char* pixel, const char* vertex);
		virtual ~Shader();

		/* A reference to an active uniform */
		struct UniformRef {
			Shader* shader;
			unsigned int type;
			std::string name;
		};

		/* Get a vector of active uniforms */
		std::vector<UniformRef> GetUniforms();
		
		/* A structure containing the individual sources for a shader */
		struct ShaderSource {
			std::string vertex;
			std::string pixel;
		}; 
		
		void Bind();
		
		/* Parse a single source into a ShaderSource */
		static ShaderSource Parse(const char* source);

		void SetUniformInt(const char* name, int value) const;
		void SetUniformFloat(const char* name, float value) const;
		void SetUniformVec2(const char* name, const vec2& value) const;
		void SetUniformVec3(const char* name, const vec3& value) const;
		void SetUniformVec4(const char* name, const vec4& value) const;
		void SetUniformMat4(const char* name, const mat4& value) const;
	};
}
