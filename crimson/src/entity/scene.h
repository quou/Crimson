#pragma once

#include "core.h"
#include "entity.h"
#include "scripting/scriptmanager.h"

namespace Crimson {
	class CR_API Scene {
	private:
		std::vector<ref<Entity>> m_entities;

		std::vector<Entity*> m_lights;

		void UpdateLights();
	public:
		ref<ScriptManager> m_scriptManager;

		Scene();

		void Update(float delta);
		void Draw(const Camera& camera) const;

		virtual ~Scene();

		Entity* CreateEntity(const std::string& name = "Untitled Entity");

		void RemoveLight(Entity* entity);

		std::vector<Entity*>* GetLights() { return &m_lights; }
		const std::vector<ref<Entity>>& GetEntities() { return m_entities; }
	};
}