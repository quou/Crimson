#include "Renderer.h"

#include <string>

#include <glad/glad.h>

#include "Logger.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Crimson {
	void GLErrorCallback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void *userParam) {
		switch (severity) {
			case GL_DEBUG_SEVERITY_HIGH: CR_LOG_FATAL_ERROR("%s", message); break;
			case GL_DEBUG_SEVERITY_MEDIUM: CR_LOG_ERROR("%s", message); break;
			case GL_DEBUG_SEVERITY_LOW: CR_LOG_WARNING("%s", message); break;
			case GL_DEBUG_SEVERITY_NOTIFICATION: CR_LOG("%s", message); break;
		}
	}

	Renderer::Renderer() {
		CR_LOG("Using OpenGL version %s", glGetString(GL_VERSION));
		CR_LOG("Renderer: %s", glGetString( GL_RENDERER));

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLErrorCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	void Renderer::Clear() {
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::DrawMesh(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh) {
		shader->Bind();

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		glm::mat4 view(1.0f);
		glm::mat4 projection = glm::perspective(45.0f, 1366.0f/768.0f, 0.0f, 100.0f);

		shader->SetMat4("u_model", model);
		shader->SetMat4("u_view", view);
		shader->SetMat4("u_projection", projection);

		mesh->Draw();
	}
}
