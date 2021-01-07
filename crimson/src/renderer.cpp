#include <GLFW/glfw3.h>

#define GLAD_GL_IMPLEMENTATION
#include "gl.h"

#include "renderer.h"

namespace Crimson {
	void Renderer::Init() {
		gladLoadGL(glfwGetProcAddress);
	}

	void Renderer::Clear(float r, float g, float b) {
		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}
