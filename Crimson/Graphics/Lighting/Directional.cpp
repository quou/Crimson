#include "Directional.h"

#include <glad/glad.h>

namespace Crimson {

   DirectionalLight::DirectionalLight() {
      m_direction = glm::vec3(0, 1, 0);
      m_color = glm::vec3(1,1,1);
   }

   DirectionalLight::DirectionalLight(const glm::vec3& color, float diffuseIntensity, const glm::vec3& direction) {
      m_direction = direction;
      m_color = color;
      m_intensity = diffuseIntensity;
   }

   void DirectionalLight::UseLight(unsigned int colorLocation, int diffuseIntensityLocation, unsigned int directionLocation) {
      glUniform3f(colorLocation, m_color.x, m_color.y, m_color.z);
      glUniform3f(directionLocation, m_direction.x, m_direction.y, m_direction.z);
      glUniform1f(diffuseIntensityLocation, m_intensity);
   }

}
