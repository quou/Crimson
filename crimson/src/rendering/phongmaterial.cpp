#include "phongmaterial.h"
#include "assets.h"

namespace Crimson {
	void PhongMaterial::Apply() {
		ref<Shader> shader = AssetManager::LoadShader(m_shader.c_str());
		shader->Bind();

		if (diffuse != "~" && !diffuse.empty()) {
			Texture* texture = AssetManager::LoadTexture(diffuse.c_str());
			if (texture) {
				texture->Bind(1);
				shader->SetUniformInt("u_material.diffuse", 1);
				shader->SetUniformBool("u_material.useDiffuse", true);
			}
		} else {
			shader->SetUniformBool("u_material.useDiffuse", false);
		}

		if (normal != "~" && !normal.empty()) {
			Texture* texture = AssetManager::LoadTexture(normal.c_str());
			if (texture) {
				texture->Bind(2);
				shader->SetUniformInt("u_material.normal", 2);
				shader->SetUniformBool("u_material.useNormal", true);
			}
		} else {
			shader->SetUniformBool("u_material.useNormal", false);
		}

		shader->SetUniformVec3("u_material.color", color);
		shader->SetUniformFloat("u_material.shininess", shininess);
	}
}