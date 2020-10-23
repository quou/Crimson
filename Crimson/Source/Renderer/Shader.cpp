#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Crimson {

	Shader::Shader(const std::string& code) {
	   Init(code);
	}

	void Shader::Init(const std::string& code) {
	   std::stringstream codeStream(code);

	   std::string vertexSource, fragmentSource, geometrySource;

	   int addingTo = -1;
	   std::string line;
	   while (std::getline(codeStream, line)) {
	      if (line == "#begin VERTEX") {
	         addingTo = 0;
	      } else if (line == "#begin FRAGMENT") {
	         addingTo = 1;
	      } else if (line == "#begin GEOMETRY") {
	         addingTo = 2;
	      } else if (line == "#end VERTEX" || line == "#end FRAGMENT" || line == "#end GEOMETRY") {
	         addingTo = -1;
	      }

	      else if (addingTo == 0) {
	         vertexSource += line + '\n';
	      } else if (addingTo == 1) {
	         fragmentSource += line + '\n';
	      } else if (addingTo == 2) {
	         geometrySource += line + '\n';
	      }
	   }

	   bool hasGeometry = !geometrySource.empty();

	   const char* vertexSourcePtr = vertexSource.c_str();
	   const char* fragmentSourcePtr = fragmentSource.c_str();
	   const char* geometrySourcePtr = geometrySource.c_str();

	   unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	   glShaderSource(vertex, 1, &vertexSourcePtr, NULL);
	   glCompileShader(vertex);

	   int r;
	   char eLog[1024];
	   glGetShaderiv(vertex, GL_COMPILE_STATUS, &r);
	   if (!r) {
	      glGetShaderInfoLog(vertex, 1024, NULL, eLog);
	      std::cout << "Error compiling the vertex shader of " << m_ID << ": " << eLog << std::endl;
	   }

	   unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	   glShaderSource(fragment, 1, &fragmentSourcePtr, NULL);
	   glCompileShader(fragment);

	   glGetShaderiv(fragment, GL_COMPILE_STATUS, &r);
	   if (!r) {
	      glGetShaderInfoLog(fragment, 1024, NULL, eLog);
	      std::cout << "Error compiling the fragment shader of " << m_ID << ": " << eLog << std::endl;
	   }

	   unsigned int geometry;
	   if (hasGeometry) {
	      geometry = glCreateShader(GL_GEOMETRY_SHADER);
	      glShaderSource(geometry, 1, &geometrySourcePtr, NULL);
	      glCompileShader(geometry);

	      glGetShaderiv(geometry, GL_COMPILE_STATUS, &r);
	      if (!r) {
	         glGetShaderInfoLog(geometry, 1024, NULL, eLog);
	         std::cout << "Error compiling the geometry shader of " << m_ID << ": " << eLog << std::endl;
	      }
	   }

	   m_ID = glCreateProgram();
	   glAttachShader(m_ID, vertex);
	   glAttachShader(m_ID, fragment);
	   if (hasGeometry) {
	      glAttachShader(m_ID, geometry);
	   }

	   glLinkProgram(m_ID);

	   glDeleteShader(vertex);
	   glDeleteShader(fragment);
	   if (hasGeometry) {
	      glDeleteShader(geometry);
	   }
	}

	Shader::~Shader() {
	   glDeleteProgram(m_ID);
	}

	Shader& Shader::Bind() {
	   glUseProgram(m_ID);
	   return *this;
	}

	void Shader::SetFloat(const char* name, float value) {
	   glUniform1f(glGetUniformLocation(m_ID, name), value);
	}

	void Shader::SetInt(const char* name, int value) {
	   glUniform1i(glGetUniformLocation(m_ID, name), value);
	}

	void Shader::SetVec2(const char* name, glm::vec2 value) {
	   glUniform2f(glGetUniformLocation(m_ID, name), value.x, value.y);
	}

	void Shader::SetVec3(const char* name, glm::vec3 value) {
	   glUniform3f(glGetUniformLocation(m_ID, name), value.x, value.y, value.z);
	}

	void Shader::SetVec4(const char* name, glm::vec4 value) {
	   glUniform4f(glGetUniformLocation(m_ID, name), value.x, value.y, value.z, value.w);
	}

	void Shader::SetMat4(const char* name, glm::mat4 value) {
	   glUniformMatrix4fv(glGetUniformLocation(m_ID, name), 1, false, glm::value_ptr(value));
	}
}
