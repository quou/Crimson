#pragma once

#include <tuple>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Crimson {
	class Camera {
	public:
		glm::mat4 projection;
		glm::vec3 position{0, 0, 0};
		glm::vec3 rotation{0, 0, 0};

		Camera() {}

		Camera(std::pair<int, int> size, float fov, float near=0.01f, float far=100.0f) {
			projection = glm::perspective(fov, (float)size.first/(float)size.second, near, far);
		}

		inline void UpdateProjection(std::pair<int, int> size, float fov, float near=0.01f, float far=100.0f) {
			projection = glm::perspective(fov, (float)size.first/(float)size.second, near, far);
		}

		inline glm::mat4 GetView() const {
			glm::mat4 rotxMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1,0,0));
			glm::mat4 rotyMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0,1,0));
			glm::mat4 rotzMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0,0,1));

			glm::mat4 positionMatrix = glm::translate(glm::mat4(1.0f), position);
			glm::mat4 rotationMatrix = rotzMatrix * rotxMatrix * rotyMatrix;

			return positionMatrix * rotationMatrix;
		}
	};
}
