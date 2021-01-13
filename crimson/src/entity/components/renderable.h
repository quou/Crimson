#pragma once

#include "rendering/model.h"
#include "memory.h"
#include "entity/entity.h"

namespace Crimson {
	class RenderableComponent : public Component {
	private:
		ref<Model> m_model;
	public:
		RenderableComponent(const ref<Model>& model);
		
		void OnInit() override;
		void OnUpdate(float delta) override;
	};
}