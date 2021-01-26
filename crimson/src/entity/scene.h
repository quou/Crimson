#pragma once

#include "core.h"
#include "entity.h"
#include "scripting/scriptmanager.h"

namespace tinyxml2 {
	class XMLPrinter;
};

namespace Crimson {
	class CR_API Scene {
	private:
		std::vector<ref<Entity>> m_entities;

		std::vector<Entity*> m_lights;

		void UpdateLights();

		friend class SceneSerialiser;
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

	class CR_API SceneSerialiser {
	private:
		ref<Scene> m_scene;
	public:
		SceneSerialiser(const ref<Scene>& scene)
			: m_scene(scene) {}

		void SerialiseEntity(const ref<Entity>& entity, tinyxml2::XMLPrinter& printer);

		void SerialiseScene(const char* path);
		void DeserialiseScene(const char* path);
	};
}