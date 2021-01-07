#pragma once

#define CR_PI 3.14159265358f

namespace Crimson {
	inline float Radians(float degrees) {
		return (float)(degrees * (CR_PI / 180.0f));
	}

	inline float Degrees(float radians) {
		return (float)(radians * (180.0f / CR_PI));
	}
}
