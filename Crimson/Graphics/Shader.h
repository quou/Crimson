#ifndef SHADER_H
#define SHADER_H

#include <string>

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

      void Bind();
      void Unbind();
   };
}

#endif /* end of include guard: SHADER_H */
