#ifndef PREFABSYSTEMS_H
#define PREFABSYSTEMS_H

#include <string>
#include <stdio.h>

#include "SceneManagement/SceneManager.h"

#include "SLECS.h"

namespace Crimson {
   struct PrefabInstancerComponent {
      std::string prefabPath;
   };

   static void InstancePrefab(PrefabInstancerComponent* instancer, ECS& ecs, SceneManager& sceneManager) {
      sceneManager.InstantiatePrefab(instancer->prefabPath, ecs);
   }

   /* THIS FUNCTION CAUSES AN INFINITE LOOP FOR AN UNKNOWN REASON */
   static void InstancePrefabs(ECS& ecs, SceneManager& sceneManager) {
     for (EntityHandle ent : System<PrefabInstancerComponent>(ecs)) {
        PrefabInstancerComponent* i = ecs.GetComponent<PrefabInstancerComponent>(ent);

         //sceneManager.InstantiatePrefab("Resources/Prefabs/Monkey-Brick.prefab", ecs);
         //sceneManager.InstantiatePrefab(ecs.GetComponent<PrefabInstancerComponent>(ent)->prefabPath, ecs);
      }
   }
}

#endif /* end of include guard: PREFABSYSTEMS_H */
