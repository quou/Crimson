#pragma once

#include "core.h"
#include "material.h"
#include "math/vec3.h"

namespace Crimson {
	class CR_API PhongMaterial : public Material {
	private:
	public:
		vec3 albedo;

		PhongMaterial(const std::string& shader, const vec3& albedo) : Material(shader), albedo(albedo) {}

		void Apply() override;
	};
}