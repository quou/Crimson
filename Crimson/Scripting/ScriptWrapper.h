#ifndef SCRIPTWRAPPER_H
#define SCRIPTWRAPPER_H

#include "chaiscript/chaiscript.hpp"
#include "ScriptComponent.h"

#include "SLECS.h"

namespace Crimson {
   namespace Scripting {
      void Execute(ScriptComponent* scriptComponent);

      void CallUpdateFunction(ScriptComponent* scriptComponent, float delta);
      void CallBeginFunction(ScriptComponent* scriptComponent);
   }
}


#endif /* end of include guard: SCRIPTWRAPPER_H */
