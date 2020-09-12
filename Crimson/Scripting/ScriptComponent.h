#ifndef SCRIPTCOMPONENT_H
#define SCRIPTCOMPONENT_H

#include "chaiscript/chaiscript.hpp"

#include <string>

namespace Crimson {
   struct ScriptComponent {
      std::string scriptFile;
      chaiscript::ChaiScript chai;
   };
}

#endif /* end of include guard: SCRIPTCOMPONENT_H */
