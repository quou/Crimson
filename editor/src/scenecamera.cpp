#include "scenecamera.h"

#include "editor.h"

namespace Crimson {
	SceneCamera::SceneCamera() {
		m_camera = Crimson::Camera(1366, 768, 45.0f, 0.1f, 1000.0f);
		m_camera.position = vec3(0.0f, 0.0f, -10.0f);
	}

	void SceneCamera::Update(vec2 viewportSize, Editor* editor)  {
		m_camera.projection = mat4::persp(70.0f, viewportSize.x/viewportSize.y, 0.1f, 1000.0f);

		if (m_firstMove) {
			m_firstMove = false;
			m_oldMouse = editor->m_window->m_eventSystem->GetMousePosition();
		}

		vec2 pos = editor->m_window->m_eventSystem->GetMousePosition();

		float changeX = pos.x - m_oldMouse.x;
		float changeY = m_oldMouse.y - pos.y;

		mouseDelta = vec2(changeX, changeY);
		m_oldMouse = pos;
	}

	void SceneCamera::Move(float delta, Editor* editor) {
		m_camera.rotation.x -= mouseDelta.y;
		m_camera.rotation.y -= mouseDelta.x;

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

		if (editor->m_window->m_eventSystem->KeyPressed(CR_KEY_W)) {
			m_camera.position += 15.0f * direction * delta;
		} else if (editor->m_window->m_eventSystem->KeyPressed(CR_KEY_S)) {
			m_camera.position -= 15.0f * direction * delta;
		}

		if (editor->m_window->m_eventSystem->KeyPressed(CR_KEY_A)) {
			m_camera.position -= 15.0f * right * delta;
		} else if (editor->m_window->m_eventSystem->KeyPressed(CR_KEY_D)) {
			m_camera.position += 15.0f * right * delta;
		}
	}
}