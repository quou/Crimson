#include "renderable.h"
#include "transform.h"
#include "rendering/shader.h"
#include "memory.h"
#include "assets.h"

namespace Crimson {
	RenderableComponent::RenderableComponent(const ref<Model>& model, const std::string& shader) 
		: m_model(model), m_shader(shader) {}

	void RenderableComponent::OnDraw(const Camera& camera) {
		TransformComponent tc = m_entity->GetComponent<TransformComponent>();

		ref<Shader> s = AssetManager::LoadShader(m_shader.c_str());
		s->Bind();

		s->SetUniformVec3("u_cameraPos", camera.position);
		s->SetUniformInt("u_pointLightCount", 1);
		s->SetUniformVec3("u_pointLights[0].position", vec3(2.0f, 0.0f, 5.0f));
		s->SetUniformVec3("u_pointLights[0].color", vec3(1.0f, 1.0f, 1.0f));
		s->SetUniformFloat("u_pointLights[0].intensity", 5.0f);

		s->SetUniformMat4("u_model", tc.GetMatrix());
		s->SetUniformMat4("u_view", camera.GetView());
		s->SetUniformMat4("u_projection", camera.projection);
		s->SetUniformVec3("u_cameraPos", camera.position);
		
		m_model->Draw();
	}
}