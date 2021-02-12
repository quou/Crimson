#pragma once

#include "core.h"
#include "memory.h"
#include "input/input.h"
#include "math/vec2.h"

struct GLFWwindow;

namespace Crimson {
	class CR_API Window {
	private:
		GLFWwindow* m_window = NULL;

		int m_width, m_height;

		friend void ResizeCallback(GLFWwindow* window, int w, int h);

		friend class ImGuiManager;
	public:
		Window(const char* title, int w, int h);
		virtual ~Window();

		void Update() const;
		void Poll() const;
		
		bool ShouldClose() const;
	
		inline int GetWidth() const { return m_width; }
		inline int GetHeight() const { return m_height; }

		void HideCursor();
		void ShowCursor();

		void SetCursorPos(const vec2& p);
	};
}
