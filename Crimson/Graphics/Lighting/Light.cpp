#include "Light.h"

#include <glad/glad.h>

namespace Crimson {
   Light::Light() {
      m_color = glm::vec3(1, 1, 1);
      m_intensity = 1.0f;
   }

   Light::Light(const glm::vec3& color, float ambientIntensity) {
      m_color = color;
      m_intensity = ambientIntensity;
   }

   void Light::UseLight(unsigned int ambientIntensityLocation, unsigned int ambientColorLocation) {
      glUniform1f(ambientIntensityLocation, m_intensity);
      glUniform3f(ambientColorLocation, m_color.x, m_color.y, m_color.z);
   }
}
