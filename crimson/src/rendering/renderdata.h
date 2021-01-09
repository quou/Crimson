#pragma once

#include "core.h"
#include "math/vec2.h"
#include "math/vec3.h"

namespace Crimson {
	struct CR_API Vertex {
		vec3 position;
		vec3 normal;
		vec2 uv;
	};
}
