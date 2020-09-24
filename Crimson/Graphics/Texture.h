#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

namespace Crimson {
   class Texture {
   private:
      unsigned int m_texture;
      std::string m_res;

      bool m_loaded{false};
   public:
      Texture() {}
      Texture(const std::string& fileName);

      void Load(const std::string& fileName);

      void Bind(unsigned int unit);

      inline std::string GetRes() const {return m_res;}
      inline bool IsLoaded() const {return m_loaded;}

      virtual ~Texture();
   };
}

#endif /* end of include guard: TEXTURE_H */
