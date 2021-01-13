#pragma once

#include "core.h"
#include "math/vec3.h"
#include "material.h"

namespace Crimson {
	/* A generic material, for a PBR shader */
	class CR_API PBRMaterial : public Material {
	private:
	public:
		vec3 albedo;
		float metallic;
		float roughness;

		PBRMaterial(const vec3& albedo, float metallic, float roughness)
			: albedo(albedo), metallic(metallic), roughness(roughness) {}

		/* Set the shader's Material uniform */
		void Apply(const ref<Shader>& shader) override;
	};
}