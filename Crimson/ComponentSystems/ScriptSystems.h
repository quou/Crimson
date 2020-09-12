#ifndef SCRIPTSYSTEMS
#define SCRIPTSYSTEMS

#include "Scripting/ScriptWrapper.h"
#include "SLECS.h"

namespace Crimson {
   static void CompileScripts(ECS& ecs) {
      int count = 1;
      for (EntityHandle ent : System<ScriptComponent>(ecs)) {
         Scripting::Execute(ecs.GetComponent<ScriptComponent>(ent));
         count++;
      }
   }

   static void InitScripts(ECS& ecs) {
      for (EntityHandle ent : System<ScriptComponent>(ecs)) {
         Scripting::CallBeginFunction(ecs.GetComponent<ScriptComponent>(ent));
      }
   }

   static void UpdateScripts(ECS& ecs, float delta) {
      for (EntityHandle ent : System<ScriptComponent>(ecs)) {
         Scripting::CallUpdateFunction(ecs.GetComponent<ScriptComponent>(ent), delta);
      }
   }
}

#endif /* end of include guard: SCRIPTSYSTEMS */
