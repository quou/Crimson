#pragma once

#include "core.h"
#include "entity/entity.h"
#include "math/vec3.h"
#include "rendering/shader.h"
#include "memory.h"

namespace Crimson {
	class PointLightComponent : public Component {
	public:
		vec3 color;
		float intensity;

		float constant = 1.0;
		float linear = 0.09;
		float quadratic = 0.032;
		
		PointLightComponent(const vec3& color, float intensity)
			: color(color), intensity(intensity) {}

		void OnInit() override;
		void OnDestroy() override;

		const vec3& GetPosition();
	};

	class SkyLightComponent : public Component {
	public:
		vec3 color;
		float intensity;

		SkyLightComponent(const vec3& color, float intensity)
			: color(color), intensity(intensity) {}

		
		void OnInit() override;
		void OnDestroy() override;
	};

	class SunComponent : public Component {
	private:
		unsigned int m_depthFB;
		unsigned int m_depthMap;

		int m_oldFB;

		const unsigned int m_shadowmapResolution = 1024;

		ref<Shader> m_shader;

		vec2 m_oldViewportSize;
	public:
		SunComponent(const vec3& direction, const vec3& color, float intensity, bool castShadows = false)
			: direction(direction), color(color), intensity(intensity), castShadows(castShadows) {}

		vec3 direction;
		vec3 color;
		float intensity;
		bool castShadows;

		void OnInit() override;
		void OnDestroy() override;

		mat4 GetTransform(const Camera& camera) const;

		void BeginShadowmapDraw(const Camera& camera);
		void EndShadowmapDraw();

		void BindShadowmap(unsigned int unit) const;
		const unsigned int GetTexture() const { return m_depthMap; }

		inline const ref<Shader>& GetShader() const { return m_shader; }
	};
}