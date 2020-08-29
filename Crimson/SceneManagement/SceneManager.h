#ifndef SCENE_MANAGER
#define SCENE_MANAGER

#include "SceneSettings.h"

namespace Crimson {
   class SceneManager {
   private:
      SceneSettings m_sceneSettings;
   public:
      inline SceneSettings* GetConfig() {return &m_sceneSettings;}
   };
}

#endif /* end of include guard: SCENE_MANAGER */
