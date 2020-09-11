#ifndef SCRIPTSYSTEMS_H
#define SCRIPTSYSTEMS_H

#include "Script/ScriptWrapper.h"
#include "SLECS.h"

namespace Crimson {
   struct ScriptComponent {
      ScriptWrapper scriptWrapper;
   };

   static void CompileScriptFromFile(ScriptComponent* script, const std::string& file) {
      script->scriptWrapper.LoadAndCompile(file);
   }

   static void RunScript(ScriptComponent* script) {
      script->scriptWrapper.Execute();
   }

   static void CallFunction(ScriptComponent* script, const std::string& prototype, Jinx::Parameters params) {
      script->scriptWrapper.RunFunction(prototype, params);
   }

   static void CallFunction(ScriptComponent* script, const std::string& prototype) {
      script->scriptWrapper.RunFunction(prototype);
   }

   static void RunScripts(ECS& ecs) {
      for (EntityHandle ent : System<ScriptComponent>(ecs)) {
         ecs.GetComponent<ScriptComponent>(ent)->scriptWrapper.Execute();
      }
   }

   static void CallFunctionInAllScripts(ECS& ecs, const std::string& prototype, Jinx::Parameters params) {
      for (EntityHandle ent : System<Transform, ScriptComponent>(ecs)) {
         ecs.GetComponent<ScriptComponent>(ent)->scriptWrapper.RunFunction(prototype, params);
      }
   }
}
#endif /* end of include guard: SCRIPTSYSTEMS_H */
