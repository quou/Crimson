#ifndef POINT_H
#define POINT_H

#include "Light.h"

class PointLight : public Light {
private:
   glm::vec3 position;

   float constant, linear, exponent;
public:
   PointLight();
};

#endif /* end of include guard: POINT_H */
