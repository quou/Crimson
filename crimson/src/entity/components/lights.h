#pragma once

#include "core.h"
#include "entity/entity.h"
#include "math/vec3.h"

namespace Crimson {
	class PointLightComponent : public Component {
	public:
		vec3 color;
		float intensity;

		PointLightComponent(const vec3& color, float intensity)
			: color(color), intensity(intensity) {}

		void OnInit() override;
		void OnDestroy() override;
	};
}