#ifndef SCENE_MANAGER
#define SCENE_MANAGER

#include "SceneSettings.h"

#include <string>
#include <vector>

#include "SLECS.h"
#include "Utils/tinyxml2.h"

#include "Graphics/Cubemap.h"
#include "Graphics/Lighting/DirectionalShadowmap.h"

#include "Scripting/ScriptWrapper.h"

#include "Graphics/Camera.h"
#include "ComponentSystems/CameraSystems.h"

namespace Crimson {
   class Renderer;

   class SceneManager {
   private:
      SceneSettings m_sceneSettings;
      std::vector<EntityHandle> m_entities;

      EntityHandle ParseEntity(tinyxml2::XMLElement* node, ECS& ecs);
      void ParseEntities(tinyxml2::XMLElement* node, ECS& ecs, EntityHandle parent=0);

      void SerializeEntity(EntityHandle ent, tinyxml2::XMLPrinter& printer, ECS& ecs);

      DirectionalShadowmap m_directionalShadowmap;

      Camera* m_currentCamera;
      std::vector<CameraComponent*> m_cameras;
   public:
      void Init();

      inline SceneSettings* GetConfig() {return &m_sceneSettings;}
      inline const std::vector<EntityHandle>& GetEntities() const {return m_entities;}
      inline Camera* GetCurrentCamera() const {return m_currentCamera;}

      void SetCurrentCamera(Camera* camera);
      void MakeCameraCurrent();

      void CreateEntity(const std::string& name, ECS& ecs);
      void DeleteEntity(EntityHandle entity, ECS& ecs);
      void DuplicateEntity(EntityHandle entity, ECS& ecs);

      void Serialize(const std::string& fileName, ECS& ecs);
      int Deserialize(const std::string& fileName, ECS& ecs);

      void CreatePrefab(const std::string& fileName, EntityHandle ent, ECS& ecs);
      void InstantiatePrefab(const std::string& fileName, ECS& ecs);

      DirectionalShadowmap* GetShadowmap() {return &m_directionalShadowmap;}

      void Update(ECS& ecs);

      Cubemap m_skybox;
   };
}

#endif /* end of include guard: SCENE_MANAGER */
