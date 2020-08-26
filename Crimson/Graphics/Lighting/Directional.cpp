#include "Directional.h"

#include <glad/glad.h>

namespace Crimson {

   DirectionalLight::DirectionalLight() : Light() {
      m_direction = glm::vec3(0, -1, 0);
   }

   DirectionalLight::DirectionalLight(const glm::vec3& color, float ambientIntensity, float diffuseIntensity, const glm::vec3& direction)
      : Light(color, ambientIntensity, diffuseIntensity){
      m_direction = direction;
   }

   void DirectionalLight::UseLight(unsigned int ambientIntensityLocation, unsigned int ambientColorLocation,
                        unsigned int diffuseIntensityLocation, unsigned int directionLocation) {
      glUniform1f(ambientIntensityLocation, m_ambientIntensity);
      glUniform3f(ambientColorLocation, m_color.x, m_color.y, m_color.z);

      glUniform3f(directionLocation, m_direction.x, m_direction.y, m_direction.z);
      glUniform1f(diffuseIntensityLocation, m_diffuseIntensity);
   }

}
