#ifndef SCRIPTCOMPONENT_H
#define SCRIPTCOMPONENT_H

#include <angelscript.h>

#include <string>

namespace Crimson {
   struct ScriptComponent {
      std::string scriptFile;
      asIScriptContext* context;
      asIScriptModule* module;
      bool compiled{false};
   };
}

#endif /* end of include guard: SCRIPTCOMPONENT_H */
