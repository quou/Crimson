#include "Light.h"

#include <glad/glad.h>

namespace Crimson {
   Light::Light() {
      m_color = glm::vec3(1, 1, 1);
      m_ambientIntensity = 1.0f;

      m_direction = glm::vec3(0, -1, 0);
      m_diffuseIntensity = 0;
   }

   Light::Light(const glm::vec3& color, float ambientIntensity, const glm::vec3& direction, float diffuseIntensity) {
      m_color = color;
      m_ambientIntensity = ambientIntensity;

      m_direction = direction;
      m_diffuseIntensity = diffuseIntensity;
   }

   void Light::UseLight(unsigned int ambientIntensityLocation, unsigned int ambientColorLocation,
                        unsigned int diffuseIntensityLocation, unsigned int directionLocation) {
      glUniform1f(ambientIntensityLocation, m_ambientIntensity);
      glUniform3f(ambientColorLocation, m_color.x, m_color.y, m_color.z);

      glUniform3f(directionLocation, m_direction.x, m_direction.y, m_direction.z);
      glUniform1f(diffuseIntensityLocation, m_diffuseIntensity);
   }
}
