#pragma once

#include "core.h"
#include "material.h"
#include "math/vec3.h"

namespace Crimson {
	class CR_API PhongMaterial : public Material {
	private:
	public:
		vec3 color;
		float shininess;

		PhongMaterial(const std::string& shader, const vec3& color, float shininess) 
			: Material(shader), color(color), shininess(shininess) {}

		void Apply() override;
	};
}