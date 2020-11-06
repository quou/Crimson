#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Crimson {
	struct AmbientLight {
		glm::vec3 color;
		float intensity;
	};

	struct DirectionalLight {
	   glm::vec3 direction;
		glm::vec3 color;
		float intensity;

		inline glm::mat4 CalculateTransform() {
			glm::mat4 projection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 1000.0f);

			return projection * glm::lookAt(-direction*10.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}
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
