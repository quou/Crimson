# Maths

There are two utility functions, `toRadians` and `toDegrees`, which will work with floats and all vector classes.

### vec2

```cpp
float vec2::x;
float vec2::y;
vec2::vec2();
vec2::vec2(float xy);
vec2::vec2(float x, float y);
```

### vec3

```cpp
float vec3::x;
float vec3::y;
float vec3::z;
vec3::vec3();
vec3::vec3(float xyz);
vec3::vec3(float x, float y, float z);
```

### vec4

```cpp
float vec4::x;
float vec4::y;
float vec4::z;
float vec4::w;
vec4::vec4();
vec4::vec4(float xyzw);
vec4::vec4(float x, float y, float z, float w);
```

### quat

`quat` Implements a standard quaternion that uses radians

```cpp
float quat::w;
float quat::x;
float quat::y;
float quat::z;
quat::quat();
quat::quat(float w, float x, float y, float z);
```
