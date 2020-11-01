#pragma once

#include <entt.hpp>

#include "Renderer/Renderer.h"
#include "Renderer/Renderer3D/LightScene.h"
#include "Physics/PhysicsScene.h"
#include "AssetManager.h"
#include "Scripting/ScriptManager.h"

#include <reactphysics3d/reactphysics3d.h>

namespace rp3d = ::reactphysics3d;

namespace Crimson {

	class Entity;

	typedef entt::entity EntityHandle;

	class Scene {
	private:
		entt::registry m_registry;

		friend class Entity;
	public:
		AssetManager m_assetManager;
		Scene();
		~Scene();

		std::shared_ptr<LightScene> m_lightScene;
		std::shared_ptr<PhysicsScene> m_physicsScene;
		std::shared_ptr<ScriptManager> m_scriptManager;

		Entity CreateEntity();

		// Events
		void Init();
		void Update(float delta);
		void Contact(rp3d::CollisionBody* body, rp3d::CollisionBody* other);

		void UpdateViewport(std::pair<int, int> size);

		inline PhysicsScene* GetPhysicsScene() {
			return &*m_physicsScene;
		}
	};

}
