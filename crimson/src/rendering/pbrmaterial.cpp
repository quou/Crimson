#include "pbrmaterial.h"
#include "assets.h"

namespace Crimson {
	void PBRMaterial::Apply() {
		ref<Shader> shader = AssetManager::LoadShader(m_shader.c_str());
		shader->Bind();

		if (!albedoMap.empty() && useAlbedo) {
			ref<Texture> ab = AssetManager::LoadTexture(albedoMap.c_str());
			ab->Bind(0);

			shader->SetUniformInt("u_useAlbedoMap", 1);
			shader->SetUniformInt("u_albedoMap", 0);
		}

		shader->SetUniformVec3("u_material.albedo", albedo);
		shader->SetUniformFloat("u_material.metallic", metallic);
		shader->SetUniformFloat("u_material.roughness", roughness);
	}
}
