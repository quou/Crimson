#pragma once

#include <glm/glm.hpp>

namespace Crimson {
	enum class UIAnchor {
		TOP,
		LEFT,
		RIGHT,
		BOTTOM,
		CENTER_VERTICAL,
		CENTER_HORIZONTAL
	};

	class UIElement {
	private:
		void Update(const glm::vec2& screenSize);

		glm::vec2 m_absolutePosition;

		int m_id = -1;

		friend class UIManager;

	public:
		UIAnchor m_anchorHorizontal{UIAnchor::LEFT};
		UIAnchor m_anchorVertical{UIAnchor::BOTTOM};
		glm::vec2 m_position;
		glm::vec2 m_size = glm::vec2(1.0f, 1.0f);

		inline unsigned int GetID() {return m_id;}
	};
}
