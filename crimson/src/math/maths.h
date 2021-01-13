#pragma once

#include "vec3.h"

#define CR_PI 3.14159265358f

namespace Crimson {
	inline static float Radians(float degrees) {
		return (float)(degrees * (CR_PI / 180.0f));
	}

	inline static vec3 Radians(vec3 degrees) {
		return vec3(Radians(degrees.x), Radians(degrees.y), Radians(degrees.z));
	}

	inline static float Degrees(float radians) {
		return (float)(radians * (180.0f / CR_PI));
	}
}
