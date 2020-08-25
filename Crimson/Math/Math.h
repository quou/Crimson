#ifndef MATH_H
#define MATH_H

#define _USE_MATH_DEFINES
#include <math.h>

namespace Crimson {
   static float ToRadians(float degrees) {
      return degrees * (M_PI / 180.0f);
   }
}

#endif /* end of include guard: MATH_H */
