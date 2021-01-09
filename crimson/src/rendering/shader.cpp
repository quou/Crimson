#include <sstream>
#include <string>

#include <glad/glad.h>

#include "shader.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include "math/mat4.h"

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
		m_panicMode = false;
	
		/* Compile the vertex shader */
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertex, NULL);
		glCompileShader(vertexShader);
		
		int success;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) { m_panicMode = true; }

		/* Compile the fragment shader */
		unsigned int pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(pixelShader, 1, &pixel, NULL);
		glCompileShader(pixelShader);

		glGetShaderiv(pixelShader, GL_COMPILE_STATUS, &success);
		if (!success) { m_panicMode = true; }

		glAttachShader(m_id, vertexShader);
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
		if (m_panicMode) { return std::vector<Shader::UniformRef>(); }

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

	void Shader::Bind() {
		if (m_panicMode) { printf("hi\n"); }

		glUseProgram(m_id);
	}

	void Shader::SetUniformInt(const char* name, int value) const {
		if (m_panicMode) { return; }

		int loc = glGetUniformLocation(m_id, name);
		glUniform1i(loc, value);
	}

	void Shader::SetUniformFloat(const char* name, float value) const {
		if (m_panicMode) { return; }

		int loc = glGetUniformLocation(m_id, name);
		glUniform1f(loc, value);
	}

	void Shader::SetUniformVec2(const char* name, const vec2& value) const {
		if (m_panicMode) { return; }

		int loc = glGetUniformLocation(m_id, name);
		glUniform2f(loc, value.x, value.y);
	}

	void Shader::SetUniformVec3(const char* name, const vec3& value) const {
		if (m_panicMode) { return; }

		int loc = glGetUniformLocation(m_id, name);
		glUniform3f(loc, value.x, value.y, value.z);
	}

	void Shader::SetUniformVec4(const char* name, const vec4& value) const {
		if (m_panicMode) { return; }

		int loc = glGetUniformLocation(m_id, name);
		glUniform4f(loc, value.x, value.y, value.z, value.w);
	}

	void Shader::SetUniformMat4(const char* name, const mat4& value) const {
		if (m_panicMode) { return; }

		int loc = glGetUniformLocation(m_id, name);
		glUniformMatrix4fv(loc, 1, GL_FALSE, value.elements);
	}
}
