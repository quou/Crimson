#ifndef DIRECTIONAL_H
#define DIRECTIONAL_H

#include "Light.h"

namespace Crimson {
   class DirectionalLight : public Light {
   private:
   public:
      glm::vec3 m_direction;

      DirectionalLight();
      DirectionalLight(const glm::vec3& color, float ambientIntensity, float diffuseIntensity, const glm::vec3& direction);

      virtual void UseLight(unsigned int ambientIntensityLocation, unsigned int ambientColorLocation,
                            unsigned int diffuseIntensityLocation, unsigned int directionLocation);
   };
}

#endif /* end of include guard: DIRECTIONAL_H */
