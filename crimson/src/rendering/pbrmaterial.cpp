#include "pbrmaterial.h"
#include "assets.h"

namespace Crimson {
	void PBRMaterial::Apply() {
		ref<Shader> shader = AssetManager::LoadShader(m_shader.c_str());
		shader->Bind();

		shader->SetUniformVec3("u_material.albedo", albedo);
		shader->SetUniformFloat("u_material.metallic", metallic);
		shader->SetUniformFloat("u_material.roughness", roughness);
	}
}