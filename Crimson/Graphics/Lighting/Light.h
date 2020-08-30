#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

namespace Crimson {
   class Light {
   protected:

   public:
      glm::vec3 m_color;
      float m_intensity;

      Light();
      Light(const glm::vec3& color, float ambientIntensity);

      virtual void UseLight(unsigned int ambientIntensityLocation, unsigned int ambientColorLocation);

      virtual ~Light() {}
   };
}

#endif /* end of include guard: LIGHT_H */
