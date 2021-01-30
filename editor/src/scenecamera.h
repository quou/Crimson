#pragma once

#include <crimson.h>

namespace Crimson {
	class Editor;

	class SceneCamera {
	private:
		Camera m_camera;

		vec2 m_oldMouse;
		vec2 mouseDelta;
		bool m_firstMove = false;
	public:
		SceneCamera();

		void Update(vec2 viewportSize, Editor* editor);
		void Move(float delta, Editor* editor);

		inline const Camera& GetCamera() const { return m_camera; }
	};
}