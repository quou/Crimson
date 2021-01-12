#include "pbrmaterial.h"

namespace Crimson {
	void PBRMaterial::Apply(const ref<Shader>& shader) {
		shader->SetUniformVec3("u_material.albedo", albedo);
		shader->SetUniformFloat("u_material.metallic", metallic);
		shader->SetUniformFloat("u_material.roughness", roughness);
	}
}