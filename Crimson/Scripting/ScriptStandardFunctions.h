#ifndef SCRIPTSTANDARDFUNCTIONS_H
#define SCRIPTSTANDARDFUNCTIONS_H

#include <angelscript.h>
#include <string>

namespace Crimson {
   namespace Scripting {
      void MessageCallback(const asSMessageInfo *msg, void *param);
      void print(std::string& msg);
   }
}

#endif /* end of include guard: SCRIPTSTANDARDFUNCTIONS_H */
