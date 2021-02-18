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
		std::string diffuse;
		std::string normal;

		PhongMaterial(const std::string& shader, const vec3& color, float shininess, const std::string& diffuse = "~", const std::string& normal = "~")
			: Material(shader, "phong"), color(color), shininess(shininess), diffuse(diffuse), normal(normal) {}

		void Apply() override;
	};
}