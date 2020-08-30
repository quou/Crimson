#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>

namespace Crimson {
   class Material {
   public:
      Material();
      Material(float specularIntensity, float shininess, std::string file = "");

      void UseMaterial(unsigned int specularIntensityLocation, unsigned int shininessLocation);

      std::string resFile;
   private:
      float m_specularIntensity;
      float m_shininess;
   };
}

#endif /* end of include guard: MATERIAL_H */
