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

		static EventSystem& instance(); 

		static void Reset();

		friend class Window;
		friend class Application;
	public:
		EventSystem();
		virtual ~EventSystem();

		static inline bool KeyPressed(int key) { return instance().m_inputs[key]; }
		static inline bool KeyJustPressed(int key) { return instance().m_downKeys[key]; }
		static inline bool KeyJustReleased(int key) { return instance().m_upKeys[key]; }

		static inline bool MouseButtonPressed(int button) { return instance().m_mouseInput[button]; }
		static inline bool MouseButtonJustPressed(int button) { return instance().m_downMouse[button]; }
		static inline bool MouseButtonJustReleased(int button) { return instance().m_upMouse[button]; }

		static const vec2& GetMousePosition() { return instance().m_mousePosition; }

		static void KeyCallback(int key, int scancode, int action, int mods);
		static void MouseButtonCallback(int button, int action, int mods);
		static void MouseMoveCallback(float x, float y);
	};
}