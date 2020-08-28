#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

namespace Crimson {
   class Texture {
   private:
      unsigned int m_texture;
   public:
      Texture() {}
      Texture(const std::string& fileName);

      void Load(const std::string& fileName);

      void Bind(unsigned int unit);

      virtual ~Texture();
   };
}

#endif /* end of include guard: TEXTURE_H */
