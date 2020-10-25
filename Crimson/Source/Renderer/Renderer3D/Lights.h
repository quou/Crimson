#pragma once

#include <glm/glm.hpp>

namespace Crimson {
	struct AmbientLight {
		float intensity;
		glm::vec3 color;
	};

	struct DirectionalLight {
	   glm::vec3 direction;
		glm::vec3 color;
		float intensity;
	};
}
