#pragma once

#include "core.h"
#include "entity.h"

namespace Crimson {
	class CR_API Scene {
	private:
		std::vector<ref<Entity>> m_entities;
	public:
		void Update(float delta);

		Entity* CreateEntity();
	};
}