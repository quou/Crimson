#pragma once

#include "core.h"

#define CR_PI 3.14159265358f

namespace Crimson {
	inline float CR_API Radians(float degrees) {
		return (float)(degrees * (CR_PI / 180.0f));
	}

	inline float CR_API Degrees(float radians) {
		return (float)(radians * (180.0f / CR_PI));
	}
}
