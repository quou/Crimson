#pragma once

#include "core.h"
#include "math/mat4.h"
#include "entity/entity.h"

namespace Crimson {
	class CR_API TransformComponent : public Component {
	public:
		vec3 translation;
		vec3 rotation;
		vec3 scale;

		TransformComponent();

		mat4 GetMatrix() const;
	};
}