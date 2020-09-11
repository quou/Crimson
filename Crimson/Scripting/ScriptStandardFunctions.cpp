#include "ScriptStandardFunctions.h"

#include <iostream>
#include <stdio.h>

namespace Crimson {
   namespace Scripting {
      void MessageCallback(const asSMessageInfo *msg, void *param) {
        const char *type = "ERR ";
        if( msg->type == asMSGTYPE_WARNING)
          type = "WARN";
        else if( msg->type == asMSGTYPE_INFORMATION )
          type = "INFO";
        printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
        std::cout << msg->section << "(" << msg->row << ", " << ") : " << msg->col << " : " << msg->message << '\n';
      }

      void print(std::string& msg) {
         std::cout << msg << '\n';
      }
   }
}
