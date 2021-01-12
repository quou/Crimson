#pragma once

#include "core.h"
#include "vec3.h"

#define CR_PI 3.14159265358f

namespace Crimson {
	inline static float CR_API Radians(float degrees) {
		return (float)(degrees * (CR_PI / 180.0f));
	}

	inline static vec3 CR_API Radians(vec3 degrees) {
		return vec3(Radians(degrees.x), Radians(degrees.y), Radians(degrees.z));
	}

	inline static float CR_API Degrees(float radians) {
		return (float)(radians * (180.0f / CR_PI));
	}
}
