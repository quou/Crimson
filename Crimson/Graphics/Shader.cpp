#include "Shader.h"


#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>

#include "Utils/FileUtils.h"

namespace Crimson {
   Shader::Shader(const std::string& vertPath, const std::string& fragPath)
      : m_vertPath(vertPath), m_fragPath(fragPath)
   {
      m_ID = Load();
   }

   Shader::~Shader() {
      glDeleteProgram(m_ID);
   }

   void Shader::Init(const std::string& vertPath, const std::string& fragPath) {
      m_vertPath = vertPath;
      m_fragPath = fragPath;
      m_ID = Load();
   }

   void Shader::Bind() {
      glUseProgram(m_ID);
   }

   void Shader::Unbind() {
      glUseProgram(0);
   }

   unsigned int Shader::GetUniformLocation(const std::string& name) {
      return glGetUniformLocation(m_ID, name.c_str());
   }

   void Shader::SetUniform1i(const std::string& name, int value) {
      glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
   }

   void Shader::SetUniformMatrix4(const std::string& name, const glm::mat4& matrix) {
      glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
   }

   void Shader::SetUniform1f(const std::string& name, float value) {
      glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
   }

   void Shader::SetUniform2f(const std::string& name, float v1, float v2) {
      glUniform2f(glGetUniformLocation(m_ID, name.c_str()), v1, v2);
   }

   void Shader::SetUniform3f(const std::string& name, float v1, float v2, float v3) {
      glUniform3f(glGetUniformLocation(m_ID, name.c_str()), v1, v2, v3);
   }

   void Shader::SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4) {
      glUniform4f(glGetUniformLocation(m_ID, name.c_str()), v1, v2, v3, v4);
   }


   unsigned int Shader::Load() {
      unsigned int program = glCreateProgram();
      unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
      unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);

      std::string v = read_file(m_vertPath);
      std::string f = read_file(m_fragPath);

      const char* vertCode = v.c_str();
      const char* fragCode = f.c_str();

      glShaderSource(vertex, 1, &vertCode, NULL);
      glCompileShader(vertex);

      int result;
      glGetShaderiv(vertex, GL_COMPILE_STATUS, &result);
      if (result == GL_FALSE) {
         int length;
         glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &length);
         std::vector<char> errorMessage(length);
         glGetShaderInfoLog(vertex, length, &length, &errorMessage[0]);
         std::cerr << &errorMessage[0] << '\n';
         glDeleteShader(vertex);
         return 0;
      }

      glShaderSource(fragment, 1, &fragCode, NULL);
      glCompileShader(fragment);

      glGetShaderiv(fragment, GL_COMPILE_STATUS, &result);
      if (result == GL_FALSE) {
         int length;
         glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &length);
         std::vector<char> errorMessage(length);
         glGetShaderInfoLog(fragment, length, &length, &errorMessage[0]);
         std::cerr << &errorMessage[0] << '\n';
         glDeleteShader(fragment);
         return 0;
      }

      glAttachShader(program, vertex);
      glAttachShader(program, fragment);

      glLinkProgram(program);
      glValidateProgram(program);

      glDeleteShader(vertex);
      glDeleteShader(fragment);

      return program;
   }
}
