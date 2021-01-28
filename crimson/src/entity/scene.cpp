#include "scene.h"
#include "components/lights.h"

namespace Crimson {
	static unsigned long NewEntityID() {
		static unsigned long id = 0;
		return id++;
	}

	Scene::Scene() {
		/* Compile scripts */
		m_scriptManager = ref<ScriptManager>(new ScriptManager());
		m_scriptManager->Compile("SceneBehaviours");
	}

	void Scene::Refresh() {
		/* Remove entities that are marked as destroyed */
		m_entities.erase(
			std::remove_if(std::begin(m_entities), std::end(m_entities), 
				[](const ref<Entity>& entity) {
					return !entity->m_alive;
				}),
			std::end(m_entities)
		);
	}

	void Scene::Update(float delta) {
		for (auto& e : m_entities) {
			e->Update(delta);
		}
	}

	void Scene::UpdateAndRefresh(float delta) {
		Refresh();
		Update(delta);
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

	void Scene::RemovePointLight(PointLightComponent* plc) {
		m_pointLights.erase(
			std::remove_if(std::begin(m_pointLights), std::end(m_pointLights), 
				[plc](PointLightComponent* p) {
					return p == plc;
				}),
			std::end(m_pointLights)
		);
	}

	void Scene::RemoveSkyLight(SkyLightComponent* slc) {
		m_skyLights.erase(
			std::remove_if(std::begin(m_skyLights), std::end(m_skyLights), 
				[slc](SkyLightComponent* s) {
					return s == slc;
				}),
			std::end(m_skyLights)
		);
	}

	Scene::~Scene() {
		/* Make sure the entities' destructors get called 
		 * before the script manager exists, to prevent a double-free */
		m_entities.clear();
	}
}