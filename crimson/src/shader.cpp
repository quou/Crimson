#include <sstream>
#include <string>

#include "shader.h"
#include "gl.h"

namespace Crimson {
	Shader::ShaderSource Shader::Parse(const char* source) {
		std::stringstream stream;
		stream << source;

		enum class ShaderType {
			NONE = -1,
			VERTEX = 0,
			PIXEL = 1,
		};

		std::string line;
		std::stringstream ss[2];
		ShaderType currentType = ShaderType::NONE;
		while (std::getline(stream, line)) {
			if (line.find("#shader") != std::string::npos) {
				if (line.find("vertex") != std::string::npos) {
					currentType = ShaderType::VERTEX;
				} else if (line.find("pixel") != std::string::npos) {
					currentType = ShaderType::PIXEL;
				}
			} else {
				if ((int)currentType == -1) { continue; }
				ss[(int)currentType] << line << "\n";
			}	
		}

		return ShaderSource{ss[0].str(), ss[1].str()};
	}

	Shader::Shader(const char* vertex, const char* pixel) {
		m_id = glCreateProgram();
	
		/* Compile the vertex shader */
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertex, NULL);
		glCompileShader(vertexShader);
		glAttachShader(m_id, vertexShader);

		/* Compile the fragment shader */
		unsigned int pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(pixelShader, 1, &pixel, NULL);
		glCompileShader(pixelShader);
		glAttachShader(m_id, pixelShader);

		/* Link the program*/
		glLinkProgram(m_id);

		/* Clean up */
		glDeleteShader(vertexShader);
		glDeleteShader(pixelShader);
	}

	Shader::~Shader() {
		/* Clean up */
		glDeleteProgram(m_id);
	}

	std::vector<Shader::UniformRef> Shader::GetUniforms() {
		int count;
		/* Get the number of uniforms */
		glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &count);

		std::vector<UniformRef> result;
		result.resize(count);

		for (int i = 0; i < count; i++) {
			char name[1024];
			unsigned int type;
			/* Get the uniform at index i */
			glGetActiveUniform(m_id, i, 1024, NULL, NULL, &type, name);
			
			result[i] = UniformRef{this, type, name};
		}

		return result;
	}
}
