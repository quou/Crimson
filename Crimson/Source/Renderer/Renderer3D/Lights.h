#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

#include "Renderer/AABB.h"

namespace Crimson {
	struct AmbientLight {
		glm::vec3 color;
		float intensity;
	};

	struct DirectionalLight {
	   glm::vec3 direction;
		glm::vec3 color;
		float intensity;

		inline glm::mat4 CalculateTransform(AABB sceneAABB) {
			// glm::mat4 projection = glm::ortho(
			// 	sceneAABB.minCorner.x, sceneAABB.maxCorner.x,
			// 	sceneAABB.minCorner.y, sceneAABB.maxCorner.y,
			// 	sceneAABB.minCorner.z, sceneAABB.maxCorner.z);
			glm::mat4 projection = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, -10000.0f, 10000.0f);

			return projection * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}

		inline glm::mat4 CalculateTransform() {
			glm::mat4 projection = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, 0.1f, 100.0f);

			return projection * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
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
