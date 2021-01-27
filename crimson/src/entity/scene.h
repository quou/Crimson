#pragma once

#include "core.h"
#include "entity.h"
#include "scripting/scriptmanager.h"

namespace tinyxml2 {
	class XMLPrinter;
};

namespace Crimson {
	class PointLightComponent;
	class SkyLightComponent;

	/* Manages creating, destroying and updating entities */
	class CR_API Scene {
	private:
		std::vector<ref<Entity>> m_entities;

		std::vector<PointLightComponent*> m_pointLights;
		std::vector<SkyLightComponent*> m_skyLights;

		void UpdateLights();

		friend class SceneSerialiser;
	public:
		ref<ScriptManager> m_scriptManager;

		Scene();

		void Update(float delta);
		void Draw(const Camera& camera) const;

		virtual ~Scene();

		Entity* CreateEntity(const std::string& name = "untitled entity");

		void RemovePointLight(PointLightComponent* plc);
		void RemoveSkyLight(SkyLightComponent* slc);

		std::vector<PointLightComponent*>* GetPointLights() { return &m_pointLights; }
		std::vector<SkyLightComponent*>* GetSkyLights() { return &m_skyLights; }
		const std::vector<ref<Entity>>& GetEntities() { return m_entities; }
	};

	/* Responsible for serialising scenes to XML files */
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