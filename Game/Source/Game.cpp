#include <Crimson.h>

class ImGuiLayer : public Crimson::Layer {
public:
	void OnInit() {

	}

	void OnUpdate(float delta) {

	}
};

class Game : public Crimson::Game {
public:
	std::shared_ptr<Crimson::Scene> m_scene;

	Crimson::Entity testEnt;
	Crimson::Entity testEnt2;
private:
	void OnInit() override {
		AddLayer<ImGuiLayer>();

		m_scene = std::make_shared<Crimson::Scene>();
		m_scene->m_lightScene->m_ambientLights.push_back({glm::vec3(1,1,1), 0.1f});
		m_scene->m_lightScene->m_directionalLights.push_back({glm::vec3(-1,-1,-1), glm::vec3(1,1,1), 1.0f});
		m_scene->m_lightScene->m_pointLights.push_back({glm::vec3(-3.5f,0,0), 1.0f, 0.09f, 0.032f, glm::vec3(1,0,0), 1.0f});

		testEnt = m_scene->CreateEntity();
		testEnt.AddComponent<Crimson::PhysicsComponent>(new Crimson::Rigidbody(m_scene->GetPhysicsScene(), glm::vec3(0, 4, 0))).rigidbody->AddBoxCollider(glm::vec3(1));
		testEnt.AddComponent<Crimson::MeshFilterComponent>("Data/MonkeyMesh.mesh");
		testEnt.AddComponent<Crimson::MaterialComponent>("Data/MonkeyMaterial.mat");

		testEnt2 = m_scene->CreateEntity();
		testEnt2.AddComponent<Crimson::PhysicsComponent>(new Crimson::Rigidbody(m_scene->GetPhysicsScene(), glm::vec3(0, -2, 0))).rigidbody->AddBoxCollider(glm::vec3(1));
		testEnt2.GetComponent<Crimson::PhysicsComponent>().rigidbody->SetKinematic(true);
		testEnt2.AddComponent<Crimson::MeshFilterComponent>("Data/CubeMesh.mesh");
		testEnt2.AddComponent<Crimson::MaterialComponent>("Data/MonkeyMaterial.mat");

		Crimson::Entity cam = m_scene->CreateEntity();
		cam.GetComponent<Crimson::TransformComponent>().position = glm::vec3(0, 0, -5);
		cam.AddComponent<Crimson::CameraComponent>(GetWindowSize(), 45.0f).active = true;
	}

	void OnUpdate(float delta) override {
		m_scene->Update(delta);
		m_scene->UpdateViewport(GetWindowSize());
	}

	void OnExit() override {
		m_scene.reset();
	}
public:
};

int main(void) {
	Game game;
	game.Run("Game", {1366, 768});
}
