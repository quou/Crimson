#include "UIElement.h"

namespace Crimson {
	static void CalculateAnchor(UIAnchor anchor, glm::vec2& absolutePosition, const glm::vec2& position, const glm::vec2& size, const glm::vec2& screenSize) {
		switch (anchor) {
		case UIAnchor::LEFT:
			absolutePosition.x = position.x;
			break;
		case UIAnchor::RIGHT:
			absolutePosition.x = (screenSize.x - position.x) - size.x;
			break;

		case UIAnchor::TOP:
			absolutePosition.y = position.y;
			break;
		case UIAnchor::BOTTOM:
			absolutePosition.y = (screenSize.y - position.y) - size.y;
			break;

		case UIAnchor::CENTER_VERTICAL:
			absolutePosition.y = ((screenSize.y / 2.0f) - (size.y / 2.0f)) + position.y;
			break;
		case UIAnchor::CENTER_HORIZONTAL:
			absolutePosition.x = ((screenSize.x / 2.0f) - (size.x / 2.0f)) + position.x;
			break;
		default:
			break;
		}
	}

	void UIElement::Update(const glm::vec2& screenSize) {
		CalculateAnchor(m_anchorHorizontal, m_absolutePosition, m_position, m_size, screenSize);
		CalculateAnchor(m_anchorVertical, m_absolutePosition, m_position, m_size, screenSize);
	}
}
