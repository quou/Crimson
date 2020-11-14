#pragma once

#include <glm/glm.hpp>

namespace Crimson {
	struct AABB {
		glm::vec3 minCorner;
		glm::vec3 maxCorner;
	};
}
