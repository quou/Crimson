#pragma once

#include <vector>
#include <memory>

#include "UIElement.h"

#include "Renderer/Shader.h"

namespace Crimson {
	class UIManager {
	private:
		std::vector<UIElement*> m_elements;

		std::shared_ptr<Shader> m_shader;

		unsigned int m_quadVA;
		unsigned int m_quadVB;

		glm::mat4 m_projection;
	public:
		UIManager();
		~UIManager();

		UIElement* AddElement();
		void RemoveElement(UIElement* element);

		void Draw();
		void Update(const glm::vec2& screenSize);
	};
}
