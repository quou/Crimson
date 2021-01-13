#pragma once

#include "rendering/model.h"
#include "memory.h"
#include "entity/entity.h"

namespace Crimson {
	class RenderableComponent : public Component {
	private:
		ref<Model> m_model;

		std::string m_shader;
	public:
		RenderableComponent(const ref<Model>& model, const std::string& shader);

		void OnDraw(const Camera& camera) override;
	};
}