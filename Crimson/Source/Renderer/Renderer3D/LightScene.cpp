#include "LightScene.h"

namespace Crimson {
	void LightScene::Apply(const Camera& camera, const std::shared_ptr<Shader>& shader) {
		shader->SetInt("u_ambientLightCount", m_ambientLights.size());
		shader->SetInt("u_directionalLightCount", m_directionalLights.size());
		shader->SetInt("u_pointLightCount", m_pointLights.size());

		shader->SetVec3("u_cameraPosition", camera.position);

		int i = 0;
		for (auto& light : m_ambientLights) {
			shader->SetFloat("u_ambientLights[" + std::to_string(i) + "].intensity", light.intensity);
			shader->SetVec3("u_ambientLights[" + std::to_string(i) + "].color", light.color);
			i++;
		}

		i = 0;
		for (auto& light : m_directionalLights) {
			shader->SetFloat("u_directionalLights[" + std::to_string(i) + "].intensity", light.intensity);
			shader->SetVec3("u_directionalLights[" + std::to_string(i) + "].direction", light.direction);
			shader->SetVec3("u_directionalLights[" + std::to_string(i) + "].color", light.color);
			i++;
		}

		i = 0;
		for (auto& light : m_pointLights) {
			shader->SetFloat("u_pointLights[" + std::to_string(i) + "].intensity", light.intensity);
			shader->SetVec3("u_pointLights[" + std::to_string(i) + "].position", light.position);
			shader->SetVec3("u_pointLights[" + std::to_string(i) + "].color", light.color);
			shader->SetFloat("u_pointLights[" + std::to_string(i) + "].constant", light.constant);
			shader->SetFloat("u_pointLights[" + std::to_string(i) + "].quadratic", light.quadratic);
			shader->SetFloat("u_pointLights[" + std::to_string(i) + "].linear", light.linear);
			i++;
		}
	}
}
