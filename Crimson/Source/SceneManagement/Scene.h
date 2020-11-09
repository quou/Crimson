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
	class RenderTarget;

	typedef entt::entity EntityHandle;

	class Scene {
	private:
		entt::registry m_registry;

		friend class Entity;

		Camera* GetMainCamera();

		void ApplyLighting();
		void RenderShadows(Camera* mainCamera);
		void RenderMeshes(Camera* mainCamera);

		void PhysicsComponentCreate(entt::registry& r, entt::entity ent);

	public:
		AssetManager m_assetManager;
		Scene(bool release);
		~Scene();

		std::shared_ptr<LightScene> m_lightScene;
		std::shared_ptr<PhysicsScene> m_physicsScene;
		std::shared_ptr<ScriptManager> m_scriptManager;

		template <class T>
		std::vector<Entity> GetEntitiesWithComponent() {
			std::vector<Entity> result;

			auto view = m_registry.view<T>();
			for (auto ent : view) {
				result.emplace_back(ent, this);
			}

			return result;
		}

		Entity CreateEntity(const std::string& name = "Unnamed Entity");
		void DestroyEntity(Entity ent);

		inline unsigned int GetShadowmap() {return m_lightScene->m_directionalShadowmaps;}

		// Events
		void Init();
		void Update(float delta);
		void Render(RenderTarget& renderTarget);
		void Render();
		void ContactStay(rp3d::CollisionBody* body, rp3d::CollisionBody* other);
		void ContactEnter(rp3d::CollisionBody* body, rp3d::CollisionBody* other);
		void ContactExit(rp3d::CollisionBody* body, rp3d::CollisionBody* other);

		void UpdateViewport(std::pair<int, int> size);

		inline PhysicsScene* GetPhysicsScene() {
			return &*m_physicsScene;
		}
	};

}
