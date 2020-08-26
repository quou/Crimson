#ifndef MATERIAL_H
#define MATERIAL_H

namespace Crimson {
   class Material {
   public:
      Material();
      Material(float specularIntensity, float shininess);

      void UseMaterial(unsigned int specularIntensityLocation, unsigned int shininessLocation);

   private:
      float m_specularIntensity;
      float m_shininess;
   };
}

#endif /* end of include guard: MATERIAL_H */
