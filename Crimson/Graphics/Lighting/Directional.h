#ifndef DIRECTIONAL_H
#define DIRECTIONAL_H

#include <glm/glm.hpp>

namespace Crimson {
   class DirectionalLight {
   private:
   public:
      glm::vec3 m_direction;
      glm::vec3 m_color;
      float m_intensity;

      DirectionalLight();
      DirectionalLight(const glm::vec3& color, float diffuseIntensity, const glm::vec3& direction);

      virtual void UseLight(unsigned int colorLocation, int diffuseIntensityLocation, unsigned int directionLocation);
   };
}

#endif /* end of include guard: DIRECTIONAL_H */
