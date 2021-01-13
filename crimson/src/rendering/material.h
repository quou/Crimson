#pragma once

#include "core.h"
#include "memory.h"
#include "rendering/shader.h"

namespace Crimson {
	/* A generic material class, for specific materials to inherit from */
	class CR_API Material {
	public:

		/* Set the shader's Material uniform */
		virtual void Apply(const ref<Shader>& shader) {}
		
		virtual ~Material() {}
	};
}