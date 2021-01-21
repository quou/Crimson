#include "phongmaterial.h"
#include "assets.h"

namespace Crimson {
	void PhongMaterial::Apply() {
		ref<Shader> shader = AssetManager::LoadShader(m_shader.c_str());
		shader->Bind();

		shader->SetUniformVec3("u_material.albedo", albedo);
	}
}