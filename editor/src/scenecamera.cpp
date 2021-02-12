#include "scenecamera.h"

#include "editor.h"

namespace Crimson {
	SceneCamera::SceneCamera() {
		m_camera = Crimson::Camera(1366, 768, 45.0f, 0.1f, 1000.0f);
		m_camera.position = vec3(0.0f, 0.0f, -10.0f);
	}

	void SceneCamera::Update(vec2 viewportSize, Editor* editor)  {
		m_camera.projection = mat4::persp(45.0f, viewportSize.x/viewportSize.y, 0.1f, 1000.0f);

		if (m_firstMove) {
			m_firstMove = false;
			m_oldMouse = EventSystem::GetMousePosition();
		}

		vec2 pos = EventSystem::GetMousePosition();

		float changeX = pos.x - m_oldMouse.x;
		float changeY = m_oldMouse.y - pos.y;

		mouseDelta = vec2(changeX, changeY);
		m_oldMouse = pos;
	}

	void SceneCamera::Move(float delta, Editor* editor) {
		m_camera.rotation.x -= mouseDelta.y * 0.1f;
		m_camera.rotation.y -= mouseDelta.x * 0.1f;

		if (m_camera.rotation.x >= 89.0f) {
			m_camera.rotation.x = 89.0f;
		}

		if (m_camera.rotation.x <= -89.0f) {
			m_camera.rotation.x = -89.0f;
		}

		vec3 rot = Radians(m_camera.rotation);

		vec3 direction(
			cos(rot.x) * sin(rot.y),
			sin(rot.x),
			cos(rot.x) * cos(rot.y)
		);

		vec3 right = direction.cross(vec3::up());

		if (EventSystem::KeyPressed(CR_KEY_W)) {
			m_camera.position += direction * 15.0f * delta;
		} else if (EventSystem::KeyPressed(CR_KEY_S)) {
			m_camera.position -= direction * 15.0f * delta;
		}

		if (EventSystem::KeyPressed(CR_KEY_A)) {
			m_camera.position -= right * 15.0f * delta;
		} else if (EventSystem::KeyPressed(CR_KEY_D)) {
			m_camera.position += right * 15.0f * delta;
		}
	}
}