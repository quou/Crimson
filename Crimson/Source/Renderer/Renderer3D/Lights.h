#pragma once

#include <glm/glm.hpp>

namespace Crimson {
	struct AmbientLight {
		glm::vec3 color;
		float intensity;
	};

	struct DirectionalLight {
	   glm::vec3 direction;
		glm::vec3 color;
		float intensity;
	};

	struct PointLight {
		glm::vec3 position;

		float constant;
		float linear;
		float quadratic;

		glm::vec3 color;
		float intensity;
	};
}
