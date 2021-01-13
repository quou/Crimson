#include "transform.h"

namespace Crimson {
	TransformComponent::TransformComponent() : m_transform(1.0f) {}

	void TransformComponent::Reset() {
		m_transform = mat4::identity();
	}

	void TransformComponent::Translate(const vec3& t) {
		m_transform *= mat4::translate(t);
	}

	void TransformComponent::Rotate(float angle, const vec3& r) {
		m_transform *= mat4::rotate(angle, r);
	}

	void TransformComponent::Scale(const vec3& s) {
		m_transform *= mat4::scale(s);
	}
}