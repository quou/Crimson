#ifndef SCRIPTWRAPPER_H
#define SCRIPTWRAPPER_H

#include <angelscript.h>
#include "ScriptComponent.h"

namespace Crimson {
   namespace Scripting {
      asIScriptEngine* CreateEngine();
      void Shutdown(asIScriptEngine* engine);
      void SetupMessageSystem(asIScriptEngine* engine);

      void RegisterFunctions(asIScriptEngine* engine);

      void InitModule(ScriptComponent* scriptComponent, asIScriptEngine* engine);
      void DeinitModule(ScriptComponent* scriptComponent, asIScriptEngine* engine);

      void CallFunction(std::string prototype, ScriptComponent* scriptComponent, asIScriptEngine* engine);

      void CallOnUpdate(ScriptComponent* scriptComponent, asIScriptEngine* engine, float delta);
   }
}


#endif /* end of include guard: SCRIPTWRAPPER_H */
