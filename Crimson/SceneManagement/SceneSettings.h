#ifndef SCENESETTINGS_H
#define SCENESETTINGS_H

#include <glm/glm.hpp>

namespace Crimson {
   struct SceneSettings {
      glm::vec3 ambientColor{1,1,1};
      glm::vec3 directionalLightPos{-10,10,-10};
      float ambientStrength{0.3f};
   };
}
#endif /* end of include guard: SCENESETTINGS_H */
