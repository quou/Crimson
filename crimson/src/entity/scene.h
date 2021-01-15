#pragma once

#include "core.h"
#include "entity.h"

namespace Crimson {
	class CR_API Scene {
	private:
		std::vector<ref<Entity>> m_entities;

		std::vector<Entity*> m_lights;

		void UpdateLights();
	public:
		void Update(float delta);
		void Draw(const Camera& camera) const;

		virtual ~Scene();

		Entity* CreateEntity();

		void RemoveLight(Entity* entity);

		std::vector<Entity*>* GetLights() { return &m_lights; }
	};
}