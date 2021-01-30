#pragma once

#include <map>

#include "core.h"
#include "keys.h"
#include "math/vec2.h"

namespace Crimson {
	class CR_API EventSystem {
	private:
		bool m_inputs[CR_KEY_COUNT];
		bool m_downKeys[CR_KEY_COUNT];
		bool m_upKeys[CR_KEY_COUNT];
		bool m_mouseInput[CR_MOUSE_BUTTON_COUNT];
		bool m_downMouse[CR_MOUSE_BUTTON_COUNT];
		bool m_upMouse[CR_MOUSE_BUTTON_COUNT];
		vec2 m_mousePosition;

		void Reset();

		friend class Window;
		friend class Application;
	public:
		EventSystem();
		virtual ~EventSystem();

		inline bool KeyPressed(int key) const { return m_inputs[key]; }
		inline bool KeyJustPressed(int key) const { return m_downKeys[key]; }
		inline bool KeyJustReleased(int key) const { return m_upKeys[key]; }

		inline bool MouseButtonPressed(int button) const { return m_mouseInput[button]; }
		inline bool MouseButtonJustPressed(int button) const { return m_downMouse[button]; }
		inline bool MouseButtonJustReleased(int button) const { return m_upMouse[button]; }

		const vec2& GetMousePosition() const { return m_mousePosition; }

		void KeyCallback(int key, int scancode, int action, int mods);
		void MouseButtonCallback(int button, int action, int mods);
		void MouseMoveCallback(float x, float y);
	};
}