#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Crimson {
   struct DirectionalLight {
      glm::vec3 direction;

      glm::vec3 ambient;
      glm::vec3 diffuse;
      glm::vec3 specular;
   };
}

#endif /* end of include guard: DIRECTIONALLIGHT_H */
