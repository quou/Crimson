#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

namespace Crimson {
   class Texture {
   private:
      unsigned int m_texture;
      std::string m_res;
   public:
      Texture() {}
      Texture(const std::string& fileName);

      void Load(const std::string& fileName);

      void Bind(unsigned int unit);

      inline std::string GetRes() const {return m_res;}

      virtual ~Texture();
   };
}

#endif /* end of include guard: TEXTURE_H */
