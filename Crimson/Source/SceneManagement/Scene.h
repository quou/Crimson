#pragma once

#include <entt.hpp>

#include "Renderer/Renderer.h"
#include "Renderer/Renderer3D/LightScene.h"
#include "Physics/PhysicsScene.h"
#include "AssetManager.h"

namespace Crimson {

	class Entity;

	class Scene {
	private:
		entt::registry m_registry;

		friend class Entity;

		AssetManager m_assetManager;
	public:
		Scene();
		~Scene();

		std::shared_ptr<LightScene> m_lightScene;
		std::shared_ptr<PhysicsScene> m_physicsScene;

		Entity CreateEntity();

		void Init();
		void Update(float delta);

		void UpdateViewport(std::pair<int, int> size);

		inline PhysicsScene* GetPhysicsScene() {
			return &*m_physicsScene;
		}
	};

}
