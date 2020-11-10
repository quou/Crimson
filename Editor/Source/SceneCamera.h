#pragma once

#include <Crimson.h>

class SceneCamera {
private:
	Crimson::Camera m_camera;
public:
	SceneCamera() {
		m_camera = Crimson::Camera({1366, 768}, 45.0f, 0.1f, 1000.0f);
	}

	void UpdateViewport(std::pair<int, int> size) {
		m_camera.UpdateProjection(size);
	}

	void Update(float delta) {
		m_camera.rotation.x += Crimson::Input::GetMouseChange().y;
		m_camera.rotation.y -= Crimson::Input::GetMouseChange().x;

		if (m_camera.rotation.x > 89.0f) {
			m_camera.rotation.x = 90.0f;
		}

		if (m_camera.rotation.x < -89.0f) {
			m_camera.rotation.x = -89.0f;
		}

		glm::vec3 rot = glm::radians(m_camera.rotation);

		glm::vec3 direction(
			cos(rot.x) * sin(rot.y),
			sin(rot.x),
			cos(rot.x) * cos(rot.y)
		);

		glm::vec3 up = glm::vec3(0, 1, 0);

		glm::vec3 right = glm::cross(direction, up);

		if (Crimson::Input::GetKey("w").pressed) {
			m_camera.position += 15.0f * direction * delta;
		} else if (Crimson::Input::GetKey("s").pressed) {
			m_camera.position -= 15.0f * direction * delta;
		}

		if (Crimson::Input::GetKey("a").pressed) {
			m_camera.position -= 15.0f * right * delta;
		} else if (Crimson::Input::GetKey("d").pressed) {
			m_camera.position += 15.0f * right * delta;
		}

	}
	void UpdateScroll(float delta) {
		glm::vec3 rot = glm::radians(m_camera.rotation);

		glm::vec3 direction(
			cos(rot.x) * sin(rot.y),
			sin(rot.x),
			cos(rot.x) * cos(rot.y)
		);

		glm::vec3 up = glm::vec3(0, 1, 0);

		glm::vec3 right = glm::cross(direction, up);

		float camSpeed = 15.0f;
		if (Crimson::Input::GetKey("shift").pressed) {
			camSpeed = 50.0f;
		}

		m_camera.position += Crimson::Input::GetScrollDelta().y * camSpeed * direction * delta;
	}

	Crimson::Camera* GetCamera() {return &m_camera;}
};
