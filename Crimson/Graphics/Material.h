#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

namespace Crimson {
   struct Material {
      glm::vec3 ambient;
      glm::vec3 diffuse;
      glm::vec3 specular;
      float shininess;
   };
}

#endif /* end of include guard: MATERIAL_H */
