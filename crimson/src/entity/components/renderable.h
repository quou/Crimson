#pragma once

#include "rendering/model.h"
#include "rendering/material.h"
#include "memory.h"
#include "entity/entity.h"

namespace Crimson {
	class RenderableComponent : public Component {
	private:
		ref<Model> m_model;
		ref<Material> m_material;

		std::string m_shader;
	public:
		RenderableComponent(const ref<Model>& model, const ref<Material>& material, const std::string& shader);

		void OnDraw(const Camera& camera) override;
	};
}