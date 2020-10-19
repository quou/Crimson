#pragma once

#include <tuple>

struct GLFWwindow;

namespace Crimson {
	class Game {
	private: 
		GLFWwindow* m_window;
	protected:
		virtual void OnInit() = 0;
		virtual void OnUpdate(float delta) = 0;
	public:
		void Run(const char* windowTitle, std::pair<int, int> windowSize);
	};
}
