#include "LightScene.h"

namespace Crimson {
	void LightScene::Apply(const Camera& camera, const std::shared_ptr<Shader>& shader) {
		shader->SetInt("u_ambientLightCount", m_ambientLights.size());
		shader->SetInt("u_directionalLightCount", m_directionalLights.size());

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
	}
}
