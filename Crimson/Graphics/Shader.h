#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <glm/glm.hpp>


namespace Crimson {
   class Shader {
   private:
      unsigned int m_ID;

      std::string m_vertPath;
      std::string m_fragPath;

      unsigned int Load();
   public:
      Shader() {m_ID = 0;}
      Shader(const std::string& vertPath, const std::string& fragPath);
      ~Shader();
      void Init(const std::string& vertPath, const std::string& fragPath);

      unsigned int GetUniformLocation(const std::string& name);

      void SetUniformMatrix4(const std::string& name, const glm::mat4& matrix);
      void SetUniform1i(const std::string& name, int value);
      void SetUniform1f(const std::string& name, float value);
      void SetUniform2f(const std::string& name, float v1, float v2);
      void SetUniform3f(const std::string& name, float v1, float v2, float v3);
      void SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4);

      void Bind();
      void Unbind();
   };
}

#endif /* end of include guard: SHADER_H */
