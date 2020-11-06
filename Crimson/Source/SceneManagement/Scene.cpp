#include "Scene.h"

#include "Entity.h"
#include "Components.h"

#include "Renderer/RenderTarget.h"

#include "Input.h"

#include <filesystem>

namespace Crimson {
	Scene::Scene(bool release) : m_assetManager(release) {
		m_lightScene = std::make_shared<LightScene>();
		m_physicsScene = std::make_shared<PhysicsScene>(this);
		m_scriptManager = std::make_shared<ScriptManager>();

		Input::Init();
		Input::LoadConfig(m_assetManager.LoadText("Data/InputConfig.conf").c_str());
	}

	Scene::~Scene() {
		auto view = m_registry.view<PhysicsComponent>();
		for (auto ent : view) {
			PhysicsComponent physics = view.get<PhysicsComponent>(ent);

			delete physics.rigidbody;
		}
	}

	void Scene::Init() {
		for (auto& p : m_assetManager.GetFilesFromDir("Data")) {
			if (p.second == ".as") {
				m_scriptManager->AddScript(m_assetManager.LoadText(p.first));
			}
		}

		m_scriptManager->Compile(m_assetManager);

		auto view = m_registry.view<ScriptComponent>();
		for (auto ent : view) {
			auto script = view.get<ScriptComponent>(ent);

			//m_scriptManager->AddBehaviour(script.className);
			m_scriptManager->SetupEntity(ent, this);
		}

		m_scriptManager->Init();
	}

	void Scene::Update(float delta) {
		m_scriptManager->Update(delta);
		m_physicsScene->Update(delta);

		auto view = m_registry.view<TransformComponent, PhysicsComponent>();
		for (auto ent : view) {
			auto [transform, physics] = view.get<TransformComponent, PhysicsComponent>(ent);

			transform.position = physics.rigidbody->GetPosition();
			transform.rotation = physics.rigidbody->GetRotation();
		}
	}

	void Scene::Render(RenderTarget& renderTarget) {
		Camera* mainCamera = GetMainCamera();

		if (mainCamera) {
			ApplyLighting();
			RenderShadows();
			renderTarget.Bind();
			RenderMeshes(mainCamera);
			renderTarget.Unbind();
		}
	}

	void Scene::Render() {
		Camera* mainCamera = GetMainCamera();

		if (mainCamera) {
			ApplyLighting();
			RenderShadows();
			RenderMeshes(mainCamera);
		}
	}

	Camera* Scene::GetMainCamera() {
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

		return mainCamera;
	}

	void Scene::ApplyLighting() {
		m_lightScene->m_ambientLights.clear();
		m_lightScene->m_directionalLights.clear();
		m_lightScene->m_pointLights.clear();
		{
			auto view = m_registry.view<TransformComponent, AmbientLightComponent>();
			for (auto ent : view) {
				auto [t, l] = view.get<TransformComponent, AmbientLightComponent>(ent);

				m_lightScene->m_ambientLights.push_back(AmbientLight{l.color, l.intensity});
			}
		}
		{
			auto view = m_registry.view<TransformComponent, DirectionalLightComponent>();
			for (auto ent : view) {
				auto [t, l] = view.get<TransformComponent, DirectionalLightComponent>(ent);

				m_lightScene->m_directionalLights.push_back(DirectionalLight{t.rotation, l.color, l.intensity});
			}
		}
		{
			auto view = m_registry.view<TransformComponent, PointLightComponent>();
			for (auto ent : view) {
				auto [t, l] = view.get<TransformComponent, PointLightComponent>(ent);

				m_lightScene->m_pointLights.push_back(PointLight{t.position, l.constant, l.linear, l.quadratic, l.color, l.intensity});
			}
		}
	}

	void Scene::RenderShadows() {
		std::vector<Mesh*> meshes;
		std::vector<glm::mat4> transforms;
		auto view = m_registry.view<TransformComponent, MeshFilterComponent, MaterialComponent>();
		for (auto ent : view) {
			auto [transform, mesh, material] = view.get<TransformComponent, MeshFilterComponent, MaterialComponent>(ent);

			meshes.push_back(m_assetManager.LoadMesh(mesh.path));
			transforms.push_back(transform.GetTransform());

		}
		Renderer::ShadowPass(*m_lightScene, transforms, meshes);
	}

	void Scene::RenderMeshes(Camera* mainCamera) {
		auto view = m_registry.view<TransformComponent, MeshFilterComponent, MaterialComponent>();
		for (auto ent : view) {
			auto [transform, mesh, material] = view.get<TransformComponent, MeshFilterComponent, MaterialComponent>(ent);

			Renderer::ShaderPass(*mainCamera, *m_lightScene, transform.GetTransform(), *m_assetManager.LoadMaterial(material.path));
			Renderer::Draw(*m_assetManager.LoadMesh(mesh.path));
		}
	}

	void Scene::UpdateViewport(std::pair<int, int> size) {
		auto view = m_registry.view<TransformComponent, CameraComponent>();
		for (auto ent : view) {
			auto [transform, camera] = view.get<TransformComponent, CameraComponent>(ent);

			camera.camera.UpdateProjection(size);
		}
	}

	Entity Scene::CreateEntity() {
		Entity ent = {m_registry.create(), this};
		ent.AddComponent<TransformComponent>();
		return ent;
	}

	void Scene::ContactStay(rp3d::CollisionBody* body, rp3d::CollisionBody* other) {
		auto view = m_registry.view<TransformComponent, PhysicsComponent, ScriptComponent>();

		Entity currentEnt, otherEnt;

		for (auto ent : view) {
			auto [transform, physics, script] = view.get<TransformComponent, PhysicsComponent, ScriptComponent>(ent);

			if (physics.rigidbody->m_body == body) {
				currentEnt = Entity(ent, this);
			} else if (physics.rigidbody->m_body == other) {
				otherEnt = Entity(ent, this);
			}
		}

		m_scriptManager->ContactStay(currentEnt.GetComponent<ScriptComponent>().id, otherEnt);
		m_scriptManager->ContactStay(otherEnt.GetComponent<ScriptComponent>().id, currentEnt);
	}

	void Scene::ContactEnter(rp3d::CollisionBody* body, rp3d::CollisionBody* other) {
		auto view = m_registry.view<TransformComponent, PhysicsComponent, ScriptComponent>();

		Entity currentEnt, otherEnt;

		for (auto ent : view) {
			auto [transform, physics, script] = view.get<TransformComponent, PhysicsComponent, ScriptComponent>(ent);

			if (physics.rigidbody->m_body == body) {
				currentEnt = Entity(ent, this);
			} else if (physics.rigidbody->m_body == other) {
				otherEnt = Entity(ent, this);
			}
		}

		m_scriptManager->ContactEnter(currentEnt.GetComponent<ScriptComponent>().id, otherEnt);
		m_scriptManager->ContactEnter(otherEnt.GetComponent<ScriptComponent>().id, currentEnt);
	}

	void Scene::ContactExit(rp3d::CollisionBody* body, rp3d::CollisionBody* other) {
		auto view = m_registry.view<TransformComponent, PhysicsComponent, ScriptComponent>();

		Entity currentEnt, otherEnt;

		for (auto ent : view) {
			auto [transform, physics, script] = view.get<TransformComponent, PhysicsComponent, ScriptComponent>(ent);

			if (physics.rigidbody->m_body == body) {
				currentEnt = Entity(ent, this);
			} else if (physics.rigidbody->m_body == other) {
				otherEnt = Entity(ent, this);
			}
		}

		m_scriptManager->ContactExit(currentEnt.GetComponent<ScriptComponent>().id, otherEnt);
		m_scriptManager->ContactExit(otherEnt.GetComponent<ScriptComponent>().id, currentEnt);
	}
}
