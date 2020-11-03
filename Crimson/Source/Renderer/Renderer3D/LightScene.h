#pragma once

#include <vector>
#include <memory>

#include "Renderer/Shader.h"
#include "Camera.h"
#include "Lights.h"

#define CR_MAX_LIGHTS 100

namespace Crimson {
	class LightScene {
	private:
		std::vector<AmbientLight> m_ambientLights;
		std::vector<DirectionalLight> m_directionalLights;
		std::vector<PointLight> m_pointLights;

		friend class Renderer;
		friend class Scene;
	public:
		float m_gamma{2.2f};

		void Apply(Shader& shader);
	};
}
