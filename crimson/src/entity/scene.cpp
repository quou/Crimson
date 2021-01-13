#include "scene.h"

namespace Crimson {
	void Scene::Update(float delta) {
		for (auto& e : m_entities) {
			e->Update(delta);
		}

		m_entities.erase(
			std::remove_if(std::begin(m_entities), std::end(m_entities), 
				[](const ref<Entity>& entity) {
					return !entity->m_alive;
				}),
			std::end(m_entities)
		);
	}

	Entity* Scene::CreateEntity() {
		Entity* e = new Entity();
		ref<Entity> r(e);
		m_entities.emplace_back(std::move(r));
		return e;
	}
}