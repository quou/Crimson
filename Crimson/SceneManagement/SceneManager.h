#ifndef SCENE_MANAGER
#define SCENE_MANAGER

#include "SceneSettings.h"

#include <string>
#include <vector>

#include "SLECS.h"
#include "Utils/tinyxml2.h"

namespace Crimson {
   class Renderer;

   class SceneManager {
   private:
      SceneSettings m_sceneSettings;
      std::vector<EntityHandle> m_entities;

      EntityHandle ParseEntity(tinyxml2::XMLElement* node, ECS& ecs);
      void ParseEntities(tinyxml2::XMLElement* node, ECS& ecs, EntityHandle parent=0);

      void SerializeEntity(EntityHandle ent, tinyxml2::XMLPrinter& printer, ECS& ecs);
   public:
      inline SceneSettings* GetConfig() {return &m_sceneSettings;}

      inline const std::vector<EntityHandle>& GetEntities() const {return m_entities;}

      void CreateEntity(const std::string& name, ECS& ecs);
      void DeleteEntity(EntityHandle entity, ECS& ecs);

      void Serialize(const std::string& fileName, ECS& ecs);
      void Deserialize(const std::string& fileName, ECS& ecs);
   };
}

#endif /* end of include guard: SCENE_MANAGER */
