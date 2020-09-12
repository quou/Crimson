#ifndef SCRIPTWRAPPER_H
#define SCRIPTWRAPPER_H

#include "ScriptComponent.h"

#include "SLECS.h"

namespace Crimson {
   namespace Scripting {
      void Execute(ECS& ecs, EntityHandle ent, ScriptComponent* scriptComponent);

      void CallUpdateFunction(ScriptComponent* scriptComponent, float delta);
      void CallBeginFunction(ScriptComponent* scriptComponent);
   }
}


#endif /* end of include guard: SCRIPTWRAPPER_H */
