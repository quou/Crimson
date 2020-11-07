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
		unsigned int m_directionalShadowmaps;
		unsigned int m_depthMapFb;

		std::vector<AmbientLight> m_ambientLights;
		std::vector<DirectionalLight> m_directionalLights;
		std::vector<PointLight> m_pointLights;
		std::shared_ptr<Shader> m_shadowmapShader;

		friend class Renderer;
		friend class Scene;

		const char* m_shadowmapShaderCode = R"(
#begin VERTEX
#version 330 core

layout (location = 0) in vec3 a_pos;

uniform mat4 u_model;
uniform mat4 u_directionalLightModel;

void main() {
	gl_Position = u_directionalLightModel * u_model * vec4(a_pos, 1.0);
}

#end VERTEX

#begin FRAGMENT
#version 330 core

void main() {}

#end FRAGMENT
		)";

	public:
		const int m_maxDirectionalLights = 10;
		float m_gamma{2.2f};

		void BindShadowmapForWrite();
		void BindShadowmapForRead(unsigned int unit);

		LightScene();
		~LightScene();

		void Apply(Camera& camera, Shader& shader);
	};
}
