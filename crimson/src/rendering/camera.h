#pragma once

#include <math.h>

#include "core.h"
#include "math/vec3.h"
#include "math/mat4.h"
#include "math/maths.h"

namespace Crimson {
	class CR_API Camera {
	public:
		float fov, near, far;

		mutable mat4 projection;
		mutable vec3 position, rotation;

		Camera() {}
		Camera(int width, int height, float fov, float near, float far) :
			fov(fov), near(near), far(far) {
			projection = mat4::persp(fov, (float)width/(float)height, near, far);
		}

		inline mat4 GetView() const {
			vec3 rot = Radians(rotation);

			vec3 direction(
				cos(rot.x) * sin(rot.y),
				sin(rot.x),
				cos(rot.x) * cos(rot.y)
			);

			return mat4::lookat(position, position + direction, vec3::up());
		}
	};
}