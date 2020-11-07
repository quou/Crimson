#pragma once

#include <memory>

#include "Renderer3D/Mesh.h"
#include "Shader.h"

#include "Renderer3D/Camera.h"

namespace Crimson {

	class Material;
	class LightScene;

	class Renderer {
	private:
		unsigned int m_gBuffer;
		unsigned int m_gPosition, m_gNormal, m_gColorSpec;
	public:
		static Renderer& instance() {
			static Renderer INSTANCE;
			return INSTANCE;
		}

		static void Init();

		static void Clear();

		static void ShadowPass(Camera& camera, LightScene& lightScene, std::vector<glm::mat4>& transform, std::vector<Mesh*>& meshes);
		static void ShaderPass(Camera& camera, LightScene& lightScene, const glm::mat4& transform, Material& material);
		static void Draw(Mesh& mesh);
	};

}
