#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

namespace Crimson {
   class Light {
   protected:

   public:
      glm::vec3 m_color;
      float m_ambientIntensity;

      float m_diffuseIntensity;
      
      Light();
      Light(const glm::vec3& color, float ambientIntensity, float diffuseIntensity);

      virtual void UseLight(unsigned int ambientIntensityLocation, unsigned int ambientColorLocation,
                            unsigned int diffuseIntensityLocation);

      virtual ~Light() {}
   };
}

#endif /* end of include guard: LIGHT_H */
