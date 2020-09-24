#ifndef SCENESETTINGS_H
#define SCENESETTINGS_H

#include <glm/glm.hpp>

#include "Graphics/Lighting/DirectionalLight.h"

namespace Crimson {
   struct SceneSettings {
      DirectionalLight directionalLight{glm::vec3(1,-1,1), glm::vec3(0.2f, 0.2f, 0.3f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f)};
   };
}
#endif /* end of include guard: SCENESETTINGS_H */
