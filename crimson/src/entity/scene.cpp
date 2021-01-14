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

	void Scene::Draw(const Camera& camera) const {
		for (auto& e : m_entities) {
			e->Draw(camera);
		}
	}

	Entity* Scene::CreateEntity() {
		ref<Entity> e(new Entity());
		e->m_scene = this;
		m_entities.emplace_back(std::move(e));
		return e.get();
	}
}