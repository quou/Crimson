#pragma once

#include <tuple>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Crimson {
	class Camera {
	private:
	public:
		float m_fov;

		float m_near;
		float m_far;

		glm::mat4 projection;
		glm::vec3 position{0, 0, 0};
		glm::vec3 rotation{0, 0, 0};

		Camera() {}

		Camera(std::pair<int, int> size, float fov, float near=0.01f, float far=100.0f) {
			m_fov = fov;
			m_near = near;
			m_far = far;
			projection = glm::perspective(fov, (float)size.first/(float)size.second, near, far);
		}

		inline void UpdateProjection(std::pair<int, int> size, float fov, float near=0.01f, float far=100.0f) {
			m_fov = fov;
			m_near = near;
			m_far = far;

			projection = glm::perspective(glm::radians(fov), (float)size.first/(float)size.second, near, far);
		}

		inline void UpdateProjection(std::pair<int, int> size) {
			projection = glm::perspective(glm::radians(m_fov), (float)size.first/(float)size.second, m_near, m_far);
		}

		inline glm::mat4 GetView() const {
			glm::vec3 rot = glm::radians(rotation);

			glm::vec3 direction(
				cos(rot.x) * sin(rot.y),
				sin(rot.x),
				cos(rot.x) * cos(rot.y)
			);

			glm::vec3 up = glm::vec3(0, 1, 0);

			glm::vec3 right = glm::cross(direction, up);

			return glm::lookAt(position, position + direction, up);
		}
	};
}
