#pragma once

#include <string>

#include "core.h"
#include "math/vec3.h"
#include "material.h"

namespace Crimson {
	/* A generic material, for a PBR shader */
	class CR_API PBRMaterial : public Material {
	private:
	public:
		std::string albedoMap;
		bool useAlbedo = true;

		vec3 albedo;
		float metallic;
		float roughness;

		PBRMaterial(const std::string& shader, const vec3& albedo,
				float metallic, float roughness, 
				const std::string& albedoMap = "")
			: Material(shader), albedo(albedo), 
			  metallic(metallic), roughness(roughness), 
			  albedoMap(albedoMap) {}

		/* Set the shader's Material uniform */
		void Apply() override;
	};
}
