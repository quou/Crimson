#include "scene.h"
#include "components/lights.h"

namespace Crimson {
	static unsigned long NewEntityID() {
		static unsigned long id = 0;
		return id++;
	}

	Scene::Scene() {
		m_scriptManager = ref<ScriptManager>(new ScriptManager());
		m_scriptManager->Compile("SceneBehaviours");
	}

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

	Entity* Scene::CreateEntity(const std::string& name) {
		ref<Entity> e(new Entity());
		e->m_scene = this;
		e->m_name = name;
		e->m_id = NewEntityID();
		m_entities.emplace_back(std::move(e));
		return e.get();
	}

	void Scene::RemoveLight(Entity* entity) {
		m_lights.erase(
			std::remove_if(std::begin(m_lights), std::end(m_lights), 
				[this, entity](Entity* e) {
					return e == entity;
				}),
			std::end(m_lights)
		);
	}


	Scene::~Scene() {
		for (auto& e : m_entities) {
			e->Destroy();
		}
		Update(1.0f);
	}
}