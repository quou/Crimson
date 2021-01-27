#pragma once

#include "core.h"
#include "entity/entity.h"
#include "math/vec3.h"

namespace Crimson {
	class PointLightComponent : public Component {
	public:
		vec3 color;
		float intensity;

		float constant = 1.0;
		float linear = 0.09;
		float quadratic = 0.032;
		
		PointLightComponent(const vec3& color, float intensity)
			: color(color), intensity(intensity) {}

		void OnInit() override;
		void OnDestroy() override;

		const vec3& GetPosition();
	};

	class SkyLightComponent : public Component {
	public:
		vec3 color;
		float intensity;

		SkyLightComponent(const vec3& color, float intensity)
			: color(color), intensity(intensity) {}

		
		void OnInit() override;
		void OnDestroy() override;
	};
}