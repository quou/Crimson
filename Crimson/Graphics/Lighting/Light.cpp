#include "Light.h"

#include <glad/glad.h>

namespace Crimson {
   Light::Light() {
      m_color = glm::vec3(1, 1, 1);
      m_ambientIntensity = 1.0f;

      m_diffuseIntensity = 0;
   }

   Light::Light(const glm::vec3& color, float ambientIntensity, float diffuseIntensity) {
      m_color = color;
      m_ambientIntensity = ambientIntensity;

      m_diffuseIntensity = diffuseIntensity;
   }

   void Light::UseLight(unsigned int ambientIntensityLocation, unsigned int ambientColorLocation,
                        unsigned int diffuseIntensityLocation) {
      glUniform1f(ambientIntensityLocation, m_ambientIntensity);
      glUniform3f(ambientColorLocation, m_color.x, m_color.y, m_color.z);

      glUniform1f(diffuseIntensityLocation, m_diffuseIntensity);
   }
}
