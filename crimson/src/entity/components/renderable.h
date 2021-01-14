#pragma once

#include "core.h"
#include "rendering/model.h"
#include "rendering/material.h"
#include "memory.h"
#include "entity/entity.h"

namespace Crimson {
	/* A renderer for the system of entities and components */
	class CR_API RenderableComponent : public Component {
	private:
		ref<Model> m_model;
	public:
		RenderableComponent(const ref<Model>& model);

		void OnDraw(const Camera& camera) override;
	};
}