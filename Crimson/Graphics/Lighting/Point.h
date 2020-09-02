#ifndef POINT_H
#define POINT_H

class PointLight {
private:
   glm::vec3 position;

   float constant, linear, exponent;
public:
   PointLight();
};

#endif /* end of include guard: POINT_H */
