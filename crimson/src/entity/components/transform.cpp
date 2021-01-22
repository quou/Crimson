#include "transform.h"

namespace Crimson {
	TransformComponent::TransformComponent() : scale(1.0f) {}

	mat4 TransformComponent::GetMatrix() const {
		mat4 transform(1.0f);

		transform *= mat4::translate(translation);
		
		mat4 rotationMatrix(1.0f);
		rotationMatrix *= mat4::rotate(rotation.x, vec3(1.0f, 0.0f, 0.0f));
		rotationMatrix *= mat4::rotate(rotation.y, vec3(0.0f, 1.0f, 0.0f));
		rotationMatrix *= mat4::rotate(rotation.z, vec3(0.0f, 0.0f, 1.0f));

		transform *= rotationMatrix;
		transform *= mat4::scale(scale);

		return transform;
	}
}