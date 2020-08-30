#ifndef SCENE_MANAGER
#define SCENE_MANAGER

#include "SceneSettings.h"

#include <string>
#include <vector>

#include "SLECS.h"

namespace Crimson {
   class Renderer;

   class SceneManager {
   private:
      SceneSettings m_sceneSettings;
      std::vector<EntityHandle> m_entities;
   public:
      inline SceneSettings* GetConfig() {return &m_sceneSettings;}

      inline const std::vector<EntityHandle>& GetEntities() const {return m_entities;}

      void Serialize(const std::string& fileName, ECS& ecs);
      void Deserialize(const std::string& fileName, ECS& ecs);
   };
}

#endif /* end of include guard: SCENE_MANAGER */
