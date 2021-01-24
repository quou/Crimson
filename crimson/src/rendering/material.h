#pragma once

#include "core.h"
#include "memory.h"
#include "rendering/shader.h"

namespace Crimson {
	/* A generic material class, for specific materials to inherit from */
	class CR_API Material {
	public:
		std::string m_shader;
		const std::string m_type;

		Material(const std::string& shader, const std::string& type) 
			: m_shader(shader), m_type(type) {}

		/* Set the shader's Material uniform */
		virtual void Apply() {}
		
		virtual ~Material() {}
	};
}