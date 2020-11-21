#pragma once

#include <memory>

#include "Renderer3D/Mesh.h"
#include "Shader.h"

#include "Renderer3D/Camera.h"

#include "AABB.h"

namespace Crimson {

	class Material;
	class LightScene;

	class Renderer {
	private:
		unsigned int m_gBuffer;
		unsigned int m_gPosition, m_gNormal, m_gColorSpec;

		float m_updateTime{0.0f};
		float m_oldTimeSinceStart{0.0f};
		float m_timeSinceStart{0.0f};

		unsigned int m_drawCallsCount;

		AABB m_sceneBound;

		std::shared_ptr<Shader> m_wireframeShader;
	public:
		static Renderer& instance() {
			static Renderer INSTANCE;
			return INSTANCE;
		}

		static void Init();
		static void DeInit();

		static void Clear();

		static void EndFrame();

		inline static float GetUpdateTime() {return instance().m_updateTime;}
		inline static unsigned int GetDrawCallsCount() {return instance().m_drawCallsCount;}

		static void ShadowPass(Camera& camera, LightScene& lightScene, std::vector<glm::mat4>& transform, std::vector<Mesh*>& meshes);
		static void ShaderPass(Camera& camera, LightScene& lightScene, const glm::mat4& transform, Material& material);
		static void Draw(Mesh& mesh);
		static void DrawWireframe(Camera& camera, const glm::mat4& transform, Mesh& mesh);
	};

}
