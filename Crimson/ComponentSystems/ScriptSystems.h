#ifndef SCRIPTSYSTEMS
#define SCRIPTSYSTEMS

#include "Scripting/ScriptWrapper.h"
#include "SLECS.h"

namespace Crimson {
   static void CompileScripts(ECS& ecs, asIScriptEngine* engine) {
      for (EntityHandle ent : System<ScriptComponent>(ecs)) {
         Scripting::InitModule(ecs.GetComponent<ScriptComponent>(ent), engine);
      }
   }

   static void DestroyScripts(ECS& ecs, asIScriptEngine* engine) {
      for (EntityHandle ent : System<ScriptComponent>(ecs)) {
         Scripting::DeinitModule(ecs.GetComponent<ScriptComponent>(ent), engine);
      }
   }

   static void InitScripts(ECS& ecs, asIScriptEngine* engine) {
      for (EntityHandle ent : System<ScriptComponent>(ecs)) {
         Scripting::CallFunction("void OnBegin()", ecs.GetComponent<ScriptComponent>(ent), engine);
      }
   }

   static void UpdateScripts(ECS& ecs, asIScriptEngine* engine, float delta) {
      for (EntityHandle ent : System<ScriptComponent>(ecs)) {
         Scripting::CallOnUpdate(ecs.GetComponent<ScriptComponent>(ent), engine, delta);
      }
   }
}

#endif /* end of include guard: SCRIPTSYSTEMS */
