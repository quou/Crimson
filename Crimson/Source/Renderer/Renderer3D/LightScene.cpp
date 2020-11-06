#include "LightScene.h"

#include <glad/glad.h>

#include "Logger.h"

namespace Crimson {
	LightScene::LightScene() {
		glGenFramebuffers(1, &m_depthMapFb);

		glGenTextures(1, &m_directionalShadowmaps);
		glBindTexture(GL_TEXTURE_2D, m_directionalShadowmaps);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024*m_maxDirectionalLights, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFb);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_directionalShadowmaps, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		m_shadowmapShader = std::make_shared<Shader>(m_shadowmapShaderCode);
	}

	LightScene::~LightScene() {
		glDeleteFramebuffers(1, &m_depthMapFb);
		glDeleteTextures(1, &m_directionalShadowmaps);
	}


	void LightScene::BindShadowmapForWrite() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFb);
	}

	void LightScene::BindShadowmapForRead(unsigned int unit) {
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, m_directionalShadowmaps);
	}

	void LightScene::Apply(Shader& shader) {
		shader.SetInt("u_ambientLightCount", m_ambientLights.size());
		shader.SetInt("u_directionalLightCount", m_directionalLights.size());
		shader.SetInt("u_pointLightCount", m_pointLights.size());
		shader.SetFloat("u_gamma", m_gamma);

		int i = 0;
		for (auto& light : m_ambientLights) {
			shader.SetFloat("u_ambientLights[" + std::to_string(i) + "].intensity", light.intensity);
			shader.SetVec3("u_ambientLights[" + std::to_string(i) + "].color", light.color);
			i++;
		}

		i = 0;
		for (auto& light : m_directionalLights) {
			if (i >= m_maxDirectionalLights) {break;}
			shader.SetFloat("u_directionalLights[" + std::to_string(i) + "].intensity", light.intensity);
			shader.SetVec3("u_directionalLights[" + std::to_string(i) + "].direction", light.direction);
			shader.SetVec3("u_directionalLights[" + std::to_string(i) + "].color", light.color);

			shader.SetMat4("u_directionalLights[" + std::to_string(i) + "].transform", light.CalculateTransform());
			shader.SetInt("u_directionalLights[" + std::to_string(i) + "].index", i);

			i++;
		}

		i = 0;
		for (auto& light : m_pointLights) {
			shader.SetFloat("u_pointLights[" + std::to_string(i) + "].intensity", light.intensity);
			shader.SetVec3("u_pointLights[" + std::to_string(i) + "].position", light.position);
			shader.SetVec3("u_pointLights[" + std::to_string(i) + "].color", light.color);
			shader.SetFloat("u_pointLights[" + std::to_string(i) + "].constant", light.constant);
			shader.SetFloat("u_pointLights[" + std::to_string(i) + "].quadratic", light.quadratic);
			shader.SetFloat("u_pointLights[" + std::to_string(i) + "].linear", light.linear);
			i++;
		}
	}
}
