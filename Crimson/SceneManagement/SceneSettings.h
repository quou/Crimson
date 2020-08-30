#ifndef SCENESETTINGS_H
#define SCENESETTINGS_H

#include "Graphics/Lighting/Directional.h"
#include "Graphics/Lighting/Light.h"

namespace Crimson {
   struct SceneSettings {
      DirectionalLight directionalLight;
      Light ambientLight;
   };
}
#endif /* end of include guard: SCENESETTINGS_H */
