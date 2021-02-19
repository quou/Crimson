#include "transform.h"

namespace Crimson {
	TransformComponent::TransformComponent() : scale(1.0f), parent(NULL) {}

	mat4 TransformComponent::GetMatrix() const {
		mat4 transform(1.0f);

		transform *= mat4::translate(m_worldTranslation);
		
		mat4 rotationMatrix(1.0f);
		rotationMatrix *= mat4::rotate(m_worldRotation.x, vec3(1.0f, 0.0f, 0.0f));
		rotationMatrix *= mat4::rotate(m_worldRotation.y, vec3(0.0f, 1.0f, 0.0f));
		rotationMatrix *= mat4::rotate(m_worldRotation.z, vec3(0.0f, 0.0f, 1.0f));

		transform *= rotationMatrix;
		transform *= mat4::scale(m_worldScale);

		return transform;
	}

	void TransformComponent::OnDraw(const Camera& camera, Shader* shader) {
		if (parent && parent->HasComponent<TransformComponent>()) {
			m_worldTranslation = parent->GetComponent<TransformComponent>()->m_worldTranslation + translation;
			m_worldRotation = parent->GetComponent<TransformComponent>()->m_worldRotation + rotation;
			m_worldScale = parent->GetComponent<TransformComponent>()->m_worldScale * scale;
		} else {
			m_worldTranslation = translation;
			m_worldRotation = rotation;
			m_worldScale = scale;
		}

	}
}