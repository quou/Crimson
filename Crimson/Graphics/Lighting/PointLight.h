#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <glm/glm.hpp>

namespace Crimson {
   struct PointLight {
      float constant;
      float linear;
      float quadratic;

      glm::vec3 ambient;
      glm::vec3 diffuse;
      glm::vec3 specular;
   };
}
#endif /* end of include guard: POINTLIGHT */
