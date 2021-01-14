#include "renderable.h"
#include "transform.h"
#include "rendering/shader.h"
#include "memory.h"
#include "assets.h"

namespace Crimson {
	RenderableComponent::RenderableComponent(const ref<Model>& model) 
		: m_model(model) {}

	void RenderableComponent::OnDraw(const Camera& camera) {
		if (!m_entity->HasComponent<TransformComponent>()) {
			Log(LogType::WARNING, "A transform component is required for rendering");
			return;
		}

		TransformComponent* tc = m_entity->GetComponent<TransformComponent>();

		m_model->SetTransform(tc->GetMatrix());	
		m_model->Draw(camera);
	}
}