#pragma once

#include "core.h"
#include "math/mat4.h"
#include "entity/entity.h"

namespace Crimson {
	class CR_API TransformComponent : public Component {
	private:
		vec3 m_worldTranslation;
		vec3 m_worldRotation;
		vec3 m_worldScale;
	public:
		vec3 translation;
		vec3 rotation;
		vec3 scale;

		Entity* parent;
		std::vector<Entity*> children;

		TransformComponent();

		void OnDraw(const Camera& camera, Shader* shader) override;

		mat4 GetMatrix() const;
	};
}