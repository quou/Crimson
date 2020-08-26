#include "Material.h"

#include <glad/glad.h>

namespace Crimson {
   Material::Material() {
      m_specularIntensity = 0.0f;
      m_shininess = 0.0f;
   }

   Material::Material(float specularIntensity, float shininess) {
      m_specularIntensity = specularIntensity;
      m_shininess = shininess;
   }

   void Material::UseMaterial(unsigned int specularIntensityLocation, unsigned int shininessLocation) {
      glUniform1f(specularIntensityLocation, m_specularIntensity);
      glUniform1f(shininessLocation, m_shininess);
   }
}
