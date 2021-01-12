#pragma once

#include "core.h"
#include "math/vec3.h"
#include "shader.h"
#include "memory.h"

namespace Crimson {
	/* A generic material, for a PBR shader */
	class CR_API PBRMaterial {
	private:
	public:
		vec3 albedo;
		float metallic;
		float roughness;

		/* Set the shader's Material uniform */
		void Apply(const ref<Shader>& shader);
	};
}