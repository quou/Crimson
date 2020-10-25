#pragma once

#include <vector>
#include <memory>

#include "Renderer/Shader.h"
#include "Camera.h"
#include "Lights.h"

#define CR_MAX_LIGHTS 100

namespace Crimson {
	class LightScene {
	public:
		std::vector<AmbientLight> m_ambientLights;
		std::vector<DirectionalLight> m_directionalLights;

		void Apply(const Camera& camera, const std::shared_ptr<Shader>& shader);
	};
}
