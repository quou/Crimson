#include <Crimson.h>

#include "EditorLayer.h"

class Game : public Crimson::Game {
public:
	std::shared_ptr<Crimson::Scene> m_scene;

	std::shared_ptr<Crimson::RenderTarget> m_renderTarget;

	Crimson::Entity cam;
	Crimson::Entity mainLight;
private:
	void OnInit() override {
		m_scene = std::make_shared<Crimson::Scene>(false);

		m_renderTarget = std::make_shared<Crimson::RenderTarget>(GetWindowSize());

		AddLayer<EditorLayer>(m_renderTarget.get());

		mainLight = m_scene->CreateEntity();
		mainLight.GetComponent<Crimson::TransformComponent>().rotation = glm::vec3(-1,-1,-1);
		mainLight.AddComponent<Crimson::AmbientLightComponent>(glm::vec3(1,1,1), 0.05f);
		mainLight.AddComponent<Crimson::DirectionalLightComponent>(glm::vec3(1,1,1), 1.0f);

		auto pinkLight = m_scene->CreateEntity();
		pinkLight.GetComponent<Crimson::TransformComponent>().position = glm::vec3(-3.5f, 0, 0);
		pinkLight.AddComponent<Crimson::PointLightComponent>(1.0f, 0.09f, 0.032f, glm::vec3(1,0,1), 1.0f);

		auto greenLight = m_scene->CreateEntity();
		greenLight.GetComponent<Crimson::TransformComponent>().position.x = 3.5f;
		greenLight.AddComponent<Crimson::PointLightComponent>(1.0f, 0.09f, 0.032f, glm::vec3(0,1,0), 1.0f);

		auto monkey = m_scene->CreateEntity();
		monkey.AddComponent<Crimson::PhysicsComponent>(new Crimson::Rigidbody(m_scene->GetPhysicsScene(), glm::vec3(0, 10, -3.5f), glm::vec3(0, 180, 0))).rigidbody->AddBoxCollider(glm::vec3(1));
		monkey.AddComponent<Crimson::MeshFilterComponent>("Data/MonkeyMesh.mesh");
		monkey.AddComponent<Crimson::MaterialComponent>("Data/MonkeyMaterial.mat");
		monkey.AddComponent<Crimson::ScriptComponent>("Monkey");

		auto monkey2 = m_scene->CreateEntity();
		monkey2.GetComponent<Crimson::TransformComponent>().position.x = -5.0f;
		monkey2.AddComponent<Crimson::PhysicsComponent>(new Crimson::Rigidbody(m_scene->GetPhysicsScene(), glm::vec3(2, 10, 3.5f), glm::vec3(0, 180, 0))).rigidbody->AddBoxCollider(glm::vec3(1));
		monkey2.AddComponent<Crimson::MeshFilterComponent>("Data/MonkeyMesh.mesh");
		monkey2.AddComponent<Crimson::MaterialComponent>("Data/MonkeyMaterial.mat");
		monkey2.AddComponent<Crimson::ScriptComponent>("Monkey");

		auto floor = m_scene->CreateEntity();
		floor.GetComponent<Crimson::TransformComponent>().scale = glm::vec3(10.0f, 0.5f, 10.0f);
		floor.AddComponent<Crimson::PhysicsComponent>(new Crimson::Rigidbody(m_scene->GetPhysicsScene(), glm::vec3(0, -2, 0)));
		floor.GetComponent<Crimson::PhysicsComponent>().rigidbody->AddBoxCollider(glm::vec3(10.0f, 0.5f, 10.0f));
		floor.GetComponent<Crimson::PhysicsComponent>().rigidbody->SetKinematic(true);
		floor.AddComponent<Crimson::MeshFilterComponent>("Data/CubeMesh.mesh");
		floor.AddComponent<Crimson::MaterialComponent>("Data/MonkeyMaterial.mat");
		floor.AddComponent<Crimson::ScriptComponent>("TestBehaviour");


		cam = m_scene->CreateEntity();
		cam.GetComponent<Crimson::TransformComponent>().position = glm::vec3(-0.7f, 0.0f, -20.5f);
		cam.GetComponent<Crimson::TransformComponent>().rotation = glm::vec3(14.5f, -25.0f, 0.0f);
		cam.AddComponent<Crimson::CameraComponent>(GetWindowSize(), 45.0f).active = true;

		m_scene->Init();
	}

	void OnUpdate(float delta) override {
		m_scene->Update(delta);
		m_scene->UpdateViewport(m_renderTarget->GetSize());
		m_scene->Render(*m_renderTarget);
   }

	void OnExit() override {
		m_renderTarget.reset();
		m_scene.reset();
	}
public:
};

int main(void) {
	Game game;
	game.Run("Crimson Editor", {1366, 768});
}
