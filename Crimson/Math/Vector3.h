#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>

namespace Crimson {
   struct Vector3 {
      float x, y, z;

      Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
      Vector3(float val) : x(val), y(val), z(val) {}
      Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

      Vector3 operator+(const Vector3& v) {
         return Vector3(x + v.x, y + v.y, z + v.z);
      }
      Vector3 operator-(const Vector3& v) {
         return Vector3(x - v.x, y - v.y, z - v.z);
      }
      Vector3 operator*(const Vector3& v) {
         return Vector3(x * v.x, y * v.y, z * v.z);
      }
      Vector3 operator/(const Vector3& v) {
         return Vector3(x / v.x, y / v.y, z / v.z);
      }

      Vector3& operator=(const Vector3& v) {
         x=v.x;
         y=v.y;
         z=v.z;
         return*this;
      }

      Vector3& operator+=(const Vector3& v) {
         x+=v.x;
         y+=v.y;
         z+=v.z;
         return *this;
      }
      Vector3& operator-=(const Vector3& v) {
         x-=v.x;
         y-=v.y;
         z-=v.z;
         return *this;
      }
      Vector3& operator*=(const Vector3& v) {
         x*=v.x;
         y*=v.y;
         z*=v.z;
         return *this;
      }
      Vector3& operator/=(const Vector3& v) {
         x/=v.x;
         y/=v.y;
         z/=v.z;
         return *this;
      }

      bool operator==(const Vector3& v) {
         return x==v.x && y==v.y && z==v.z;
      }
      bool operator!=(const Vector3& v) {
         return x!=v.x && y!=v.y && z!=v.z;
      }

      Vector3 operator*(float scalar) {
         return Vector3(x*scalar,y*scalar,z*scalar);
      }
      Vector3& operator*=(float scalar) {
         x*=scalar;
         y*=scalar;
         z*=scalar;
         return *this;
      }

      Vector3& Inverse() {
         x = -x;
         y = -y;
         z = -z;
         return *this;
      }

      float Dot() const {
         float dot = 0.0f;

         dot+=x;
         dot+=y;
         dot+=z;

         return dot;
      }

      float Dot(const Vector3& v) const {
         float dot = 0.0f;

         dot+=x*v.x;
         dot+=y*v.y;
         dot+=z*v.z;

         return dot;
      }

      Vector3 Normalized() const {
         double mag = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
      	float nx = x / mag;
      	float ny = y / mag;
      	float nz = z / mag;
         return Vector3(nx, ny, nz);
      }
      Vector3& Normalize() {
         double mag = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
      	x = x / mag;
      	y = y / mag;
      	z = z / mag;
         return *this;
      }

      Vector3 Lerp(Vector3 end, float t) {
         Vector3 start = *this;
         Vector3 result = ((end - start) * start + t);
         x = result.x;y = result.y;z = result.z;
         return result;
      }

      float Magnitude() {
         return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
      }

      Vector3 Cross(const Vector3& v) const {
         float a1 = x;
         float a2 = y;
         float a3 = z;
         float b1 = v.x;
         float b2 = v.y;
         float b3 = v.z;

         return Vector3(a2 * b3 - a3 * b2, a3 * b1 - a1 * b3, a1 * b2 - a2 * b1);
      }

      friend std::ostream& operator<<(std::ostream& os, const Vector3& v);
   };

   std::ostream& operator<<(std::ostream& os, const Vector3& v) {
      os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
      return os;
   }
}

#endif /* end of include guard: VECTOR2_H */
