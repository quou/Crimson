#pragma once

#include "core.h"
#include "memory.h"
#include "rendering/shader.h"

namespace Crimson {
	/* A generic material class, for specific materials to inherit from */
	class CR_API Material {
	public:
		std::string m_shader;

		Material(const std::string& shader) : m_shader(shader) {}

		/* Set the shader's Material uniform */
		virtual void Apply() {}
		
		virtual ~Material() {}
	};
}