#include "renderable.h"

namespace Crimson {
	RenderableComponent::RenderableComponent(const ref<Model>& model) : m_model(model) {

	}

	void RenderableComponent::OnInit() {

	}

	void RenderableComponent::OnUpdate(float delta) {
		m_model->Draw();
	}
}