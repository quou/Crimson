#ifndef WORKING_DIR_H
#define WORKING_DIR_H

#include <string>

#ifdef WINDOWS
#include <Windows.h>

namespace Crimson {
   static std::string GetWorkingDir()
   {
     char result[ MAX_PATH ];
     return std::string( result, GetModuleFileName( NULL, result, MAX_PATH ) );
   }`
}

#else
#include <limits.h>
#include <unistd.h>

namespace Crimson{
   static std::string GetWorkingDir()
   {
     char result[ PATH_MAX ];
     ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
     return std::string( result, (count > 0) ? count : 0 );
   }
}
#endif

#endif /* end of include guard: WORKING_DIR_H */
