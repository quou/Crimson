#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

namespace Crimson {
   class Light {
   private:
      glm::vec3 m_color;
      float m_ambientIntensity;

      glm::vec3 m_direction;
      float m_diffuseIntensity;

   public:
      Light();
      Light(const glm::vec3& color, float ambientIntensity, const glm::vec3& direction, float diffuseIntensity);

      virtual void UseLight(unsigned int ambientIntensityLocation, unsigned int ambientColorLocation,
                            unsigned int diffuseIntensityLocation, unsigned int directionLocation);

      virtual ~Light() {}
   };
}

#endif /* end of include guard: LIGHT_H */
