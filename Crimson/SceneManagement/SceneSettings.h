#ifndef SCENESETTINGS_H
#define SCENESETTINGS_H

#include <glm/glm.hpp>

namespace Crimson {
   struct SceneSettings {
      glm::vec3 directionalLightDir{1,-1,1};
   };
}
#endif /* end of include guard: SCENESETTINGS_H */
