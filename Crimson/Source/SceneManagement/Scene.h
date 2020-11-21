#pragma once

#include <entt.hpp>

#include "Renderer/Renderer.h"
#include "Renderer/Renderer3D/LightScene.h"
#include "Renderer/Renderer3D/Skybox.h"
#include "Physics/PhysicsScene.h"
#include "AssetManager.h"
#include "Scripting/ScriptManager.h"

#include "GUID.h"

#include <reactphysics3d/reactphysics3d.h>

namespace rp3d = ::reactphysics3d;

namespace Crimson {

	class Entity;
	class RenderTarget;
	struct MeshFilterComponent;

	typedef entt::entity EntityHandle;

	struct SceneConfig {
		std::string name{"Untitled Scene"};

		std::string skyboxPosX;
		std::string skyboxNegX;
		std::string skyboxPosY;
		std::string skyboxNegY;
		std::string skyboxPosZ;
		std::string skyboxNegZ;
	};

	class Scene {
	private:
		entt::registry m_registry;

		friend class Entity;
		friend class SceneSerialiser;

		Camera* GetMainCamera();

		void ApplyLighting();
		void RenderShadows(Camera* mainCamera);
		void RenderMeshes(Camera* mainCamera);

		void PhysicsComponentCreate(entt::registry& r, entt::entity ent);

		SceneConfig m_config;

		std::shared_ptr<Skybox> m_skybox;

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

		Entity CreateEntity(const std::string& name = "Unnamed Entity", const std::string& tag = "Default", const GUID& guid="");
		void DestroyEntity(Entity ent);

		inline unsigned int GetShadowmap() {return m_lightScene->m_directionalShadowmaps;}

		void LoadSkybox();

		// Events
		void Init();
		void Update(float delta);
		void Render(RenderTarget& renderTarget);
		void Render(RenderTarget& renderTarget, Camera* camera);
		void DrawWireframe(Entity ent);
		void Render();
		void ContactStay(rp3d::CollisionBody* body, rp3d::CollisionBody* other);
		void ContactEnter(rp3d::CollisionBody* body, rp3d::CollisionBody* other);
		void ContactExit(rp3d::CollisionBody* body, rp3d::CollisionBody* other);

		float GetScriptEngineUpdateTime();
		float GetPhysicsEngineUpdateTime();

		void UpdateViewport(std::pair<int, int> size);

		inline PhysicsScene* GetPhysicsScene() {
			return &*m_physicsScene;
		}

		inline SceneConfig& GetConfig() {return m_config;}
	};

}
