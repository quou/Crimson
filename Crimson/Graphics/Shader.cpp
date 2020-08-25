#include "Shader.h"

#include <glad/glad.h>

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
         glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &length);
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
