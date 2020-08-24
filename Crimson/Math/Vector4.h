#ifndef VECTOR4_H
#define VECTOR4_H

#include <iostream>

namespace Crimson {
   struct Vector4 {
      float x, y, z, w;

      Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
      Vector4(float val) : x(val), y(val), z(val), w(val) {}
      Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

      Vector4 operator+(const Vector4& v) {
         return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
      }
      Vector4 operator-(const Vector4& v) {
         return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
      }
      Vector4 operator*(const Vector4& v) {
         return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
      }
      Vector4 operator/(const Vector4& v) {
         return Vector4(x / v.x, y / v.y, z / v.z, w / v.w);
      }

      Vector4& operator=(const Vector4& v) {
         x=v.x;
         y=v.y;
         z=v.z;
         w=v.w;
         return*this;
      }

      Vector4& operator+=(const Vector4& v) {
         x+=v.x;
         y+=v.y;
         z+=v.z;
         w+=v.w;
         return *this;
      }
      Vector4& operator-=(const Vector4& v) {
         x-=v.x;
         y-=v.y;
         z-=v.z;
         w-=v.w;
         return *this;
      }
      Vector4& operator*=(const Vector4& v) {
         x*=v.x;
         y*=v.y;
         z*=v.z;
         w*=v.w;
         return *this;
      }
      Vector4& operator/=(const Vector4& v) {
         x/=v.x;
         y/=v.y;
         z/=v.z;
         w/=v.w;
         return *this;
      }

      bool operator==(const Vector4& v) {
         return x==v.x && y==v.y && z==v.z && w == v.w;
      }
      bool operator!=(const Vector4& v) {
         return x!=v.x && y!=v.y && z!=v.z && w != v.w;
      }

      Vector4 operator*(float scalar) {
         return Vector4(x*scalar,y*scalar,z*scalar,w*scalar);
      }
      Vector4& operator*=(float scalar) {
         x*=scalar;
         y*=scalar;
         z*=scalar;
         w*=scalar;
         return *this;
      }

      Vector4& Inverse() {
         x = -x;
         y = -y;
         z = -z;
         w = -z;
         return *this;
      }

      float Dot() const {
         float dot = 0.0f;

         dot+=x;
         dot+=y;
         dot+=z;
         dot+=w;

         return dot;
      }

      float Dot(const Vector4& v) const {
         float dot = 0.0f;

         dot+=x*v.x;
         dot+=y*v.y;
         dot+=z*v.z;
         dot+=w*v.w;

         return dot;
      }

      Vector4 Normalized() const {
         double mag = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2) + pow(w, 2));
      	float nx = x / mag;
      	float ny = y / mag;
      	float nz = z / mag;
      	float nw = z / mag;
         return Vector4(nx, ny, nz, nw);
      }
      Vector4& Normalize() {
         double mag = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2) + pow(w, 2));
      	x = x / mag;
      	y = y / mag;
      	z = z / mag;
      	w = w / mag;
         return *this;
      }

      Vector4 Lerp(Vector4 end, float t) {
         Vector4 start = *this;
         Vector4 result = ((end - start) * start + t);
         x = result.x;y = result.y;z = result.z;w = result.w;
         return result;
      }

      float Magnitude() {
         return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2) + pow(w, 2));
      }

      friend std::ostream& operator<<(std::ostream& os, const Vector4& v);
   };

   std::ostream& operator<<(std::ostream& os, const Vector4& v) {
      os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
      return os;
   }
}

#endif /* end of include guard: VECTOR2_H */
