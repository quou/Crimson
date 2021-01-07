#pragma once

#include <vector>

namespace Crimson {
	/* A generic GLSL shader class */
	class Shader {
	private:
		unsigned int m_id;
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
		
		/* Parse a single source into a ShaderSource */
		static ShaderSource Parse(const char* source);
	};
}
