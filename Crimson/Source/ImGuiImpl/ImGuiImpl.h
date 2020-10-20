#pragma once

struct GLFWwindow;

namespace Crimson {
	class ImGuiImpl {
	public:
		static void Init(GLFWwindow* window);
		static void Quit();
		static void BeginFrame();
		static void EndFrame();
	};
}
