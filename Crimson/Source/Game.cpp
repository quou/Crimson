#include "Game.h"

#include "Logger.h"

#include <GLFW/glfw3.h>

namespace Crimson {
	void Game::Run(const char* windowTitle, std::pair<int, int> windowSize) {
		CR_ASSERT(glfwInit(), "%s", "Unable to initialise GLFW"); 
	}
}
