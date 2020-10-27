#include "Scene.h"

#include "Entity.h"

#include "Components.h"

namespace Crimson {
	Scene::Scene() {
		m_lightScene = std::make_shared<LightScene>();
	}

	Scene::~Scene() {

	}

	void Scene::Update(float delta) {
		Camera* mainCamera;
		{
			auto view = m_registry.view<TransformComponent, CameraComponent>();
			for (auto ent : view) {
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(ent);

				camera.camera.position = transform.position;
				camera.camera.rotation = transform.rotation;

				if (camera.active) {
					mainCamera = &camera.camera;
					break;
				}
			}
		}

		if (mainCamera) {
			auto view = m_registry.view<TransformComponent, MeshFilterComponent, MaterialComponent>();
			for (auto ent : view) {
				auto [transform, mesh, material] = view.get<TransformComponent, MeshFilterComponent, MaterialComponent>(ent);

				Renderer::Draw(*mainCamera, *m_lightScene, transform.GetTransform(), material.material, mesh.mesh);
			}
		}
	}

	Entity Scene::CreateEntity() {
		Entity ent = {m_registry.create(), this};
		ent.AddComponent<TransformComponent>();
		return ent;
	}
}
