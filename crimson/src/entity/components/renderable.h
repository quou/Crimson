#pragma once

#include "core.h"
#include "rendering/model.h"
#include "rendering/material.h"
#include "memory.h"
#include "entity/entity.h"

namespace Crimson {
	/* A renderer for the system of entities and components */
	class CR_API RenderableComponent : public Component {
	public:
		ref<Model> m_model;

		RenderableComponent(const ref<Model>& model);

		void OnDraw(const Camera& camera) override;
	};
}