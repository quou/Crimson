#include <glad/glad.h>

#include "lights.h"
#include "entity/scene.h"
#include "transform.h"

namespace Crimson {
	void PointLightComponent::OnInit() {
		if (!m_entity->HasComponent<TransformComponent>()) {
			Log(LogType::WARNING, "Point Light requires the entity to have a transform component");
		}

		m_entity->m_scene->GetPointLights()->push_back(this);
	}

	void PointLightComponent::OnDestroy() {
		m_entity->m_scene->RemovePointLight(this);
	}


	const vec3& PointLightComponent::GetPosition() {
		if (!m_entity->HasComponent<TransformComponent>()) {
			return vec3();
		}

		return m_entity->GetComponent<TransformComponent>()->translation;
	}

	void SkyLightComponent::OnInit() {
		m_entity->m_scene->GetSkyLights()->push_back(this);
	}

	void SkyLightComponent::OnDestroy() {
		m_entity->m_scene->RemoveSkyLight(this);
	}

	void SunComponent::OnInit() {
		m_shader = ref<Shader>(new Shader(
			R"(
				#version 330 core
				layout (location = 0) in vec3 a_position;

				uniform mat4 u_lightTransform;
				uniform mat4 u_model;

				void main() {
					gl_Position = u_lightTransform * u_model * vec4(a_position, 1.0);
				}
			)",
			R"(
				#version 330 core

				void main() {}
			)"
		));

		glGenFramebuffers(1, &m_depthFB);

		glGenTextures(1, &m_depthMap);
		glBindTexture(GL_TEXTURE_2D, m_depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
			m_shadowmapResolution, m_shadowmapResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindFramebuffer(GL_FRAMEBUFFER, m_depthFB);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void SunComponent::OnDestroy() {
		glDeleteFramebuffers(1, &m_depthFB);
		glDeleteTextures(1, &m_depthMap);
	}


	mat4 SunComponent::GetTransform() const {
		mat4 lightProjection = mat4::ortho(-20.0f, 20.0f, -20.0f, 20.0f, -10000.0f, 10000.0f);

		mat4 lightView = mat4::lookat(-direction, 
									  vec3(0.0f, 0.0f,  0.0f), 
						 			  vec3(0.0f, 1.0f,  0.0f));

		return lightProjection * lightView;
	}

	void SunComponent::BeginShadowmapDraw() {
		/* We need to get & store the original viewport and size, for when
		 * the framebuffer is unbound and the viewport is reset, it is easier to
		 * do this than passing in the originals, and has almost zero cost to
		 * performance */
		/* Get the old viewport */
		int v[4];
		glGetIntegerv(GL_VIEWPORT, v);
		m_oldViewportSize = vec2(v[2], v[3]);

		/* Get the old framebuffer */
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &m_oldFB);


		glViewport(0, 0, m_shadowmapResolution, m_shadowmapResolution);
		glBindFramebuffer(GL_FRAMEBUFFER, m_depthFB);
		glClear(GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT);
	
		m_shader->Bind();
		m_shader->SetUniformMat4("u_lightTransform", GetTransform());
	}

	void SunComponent::EndShadowmapDraw() {
		glViewport(0, 0, m_oldViewportSize.x, m_oldViewportSize.y);

		glBindFramebuffer(GL_FRAMEBUFFER, m_oldFB);
		glCullFace(GL_BACK);
	}

	void SunComponent::BindShadowmap(unsigned int unit) const {
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, m_depthMap);
	}
}