#pragma once

#include "core.h"

struct GLFWwindow;

namespace Crimson {
	class CR_API Window {
	private:
		GLFWwindow* m_window;

		int m_width, m_height;

		friend void ResizeCallback(GLFWwindow* window, int w, int h);
	public:
		Window(const char* title, int w, int h);
		virtual ~Window();

		void Update() const;
		
		bool ShouldClose() const;
	
		inline int GetWidth() const { return m_width; }
		inline int GetHeight() const { return m_height; }
	};
}
