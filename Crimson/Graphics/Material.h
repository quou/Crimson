#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

namespace Crimson {
   struct Material {
      glm::vec3 color;
      float specularStrength;
   };
}

#endif /* end of include guard: MATERIAL_H */
