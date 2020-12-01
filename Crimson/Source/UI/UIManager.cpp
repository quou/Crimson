#include "UIManager.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Logger.h"

namespace Crimson {
	static const char* elementShader = R"(
#begin VERTEX

#version 330 core

layout (location = 0) in vec4 a_vertex;

uniform mat4 u_projection;
uniform mat4 u_model;

void main() {
	gl_Position = u_projection * u_model * vec4(a_vertex.xy, 0.0, 1.0);
}

#end VERTEX

#begin FRAGMENT

#version 330 core

out vec4 v_fragColor;

void main() {
	v_fragColor = vec4(1.0);
}

#end FRAGMENT
	)";

	UIManager::UIManager() {
		m_shader = std::make_shared<Shader>(elementShader);

		float vertices[] = {
           // pos      // tex
           0.0f, 1.0f, 0.0f, 1.0f,
           1.0f, 0.0f, 1.0f, 0.0f,
           0.0f, 0.0f, 0.0f, 0.0f,

           0.0f, 1.0f, 0.0f, 1.0f,
           1.0f, 1.0f, 1.0f, 1.0f,
           1.0f, 0.0f, 1.0f, 0.0f
       };

		 glGenVertexArrays(1, &m_quadVA);
       glGenBuffers(1, &m_quadVB);

       glBindBuffer(GL_ARRAY_BUFFER, m_quadVB);
       glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

       glBindVertexArray(m_quadVA);
       glEnableVertexAttribArray(0);
       glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
       glBindBuffer(GL_ARRAY_BUFFER, 0);
       glBindVertexArray(0);
	}

	UIManager::~UIManager() {
		for (auto e : m_elements) {
			delete e;
		}

		glDeleteVertexArrays(1, &m_quadVA);
		glDeleteBuffers(1, &m_quadVB);
	}

	UIElement* UIManager::AddElement() {
		auto n = new UIElement();
		n->m_id = m_elements.size();
		m_elements.push_back(n);
		return n;
	}

	void UIManager::RemoveElement(UIElement* element) {
		element->m_id = -1;
	}

	void UIManager::Draw() {
		for (auto el : m_elements) {
			m_shader->Bind();

			glm::mat4 model(1.0f);
			model = glm::translate(model, glm::vec3(el->m_absolutePosition, 0.0f));
			model = glm::scale(model, glm::vec3(el->m_size, 1.0f));

			m_shader->SetMat4("u_model", model);
			m_shader->SetMat4("u_projection", m_projection);

			glBindVertexArray(m_quadVA);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}

	void UIManager::Update(const glm::vec2& screenSize) {
		m_projection = glm::ortho(0.0f, screenSize.x, screenSize.y, 0.0f, -1.0f, 1.0f);

		auto it = m_elements.begin();
		while (it != m_elements.end()) {
			(*it)->Update(screenSize);

			if ((*it)->m_id <= -1) {
				delete (*it);
				it = m_elements.erase(it);
			} else {
				it++;
			}
		}
	}
}
