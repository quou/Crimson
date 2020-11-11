#include <Crimson.h>

#include "EditorLayer.h"

#include "SceneCamera.h"

class Game : public Crimson::Game {
public:
	std::shared_ptr<Crimson::Scene> m_scene;

	std::shared_ptr<Crimson::RenderTarget> m_renderTarget;

	Crimson::Entity cam;
	Crimson::Entity mainLight;

	SceneCamera m_camera;
private:
	void OnInit() override {
		Crimson::Input::RegisterKey("shift", CR_KEY_LEFT_SHIFT);

		m_scene = std::make_shared<Crimson::Scene>(false);

		m_renderTarget = std::make_shared<Crimson::RenderTarget>(GetWindowSize());

		AddLayer<EditorLayer>(&m_camera, m_renderTarget.get(), m_scene.get());

		// mainLight = m_scene->CreateEntity("Main light");
		// mainLight.GetComponent<Crimson::TransformComponent>().rotation = glm::vec3(-1,-1,-1);
		// mainLight.AddComponent<Crimson::AmbientLightComponent>(glm::vec3(1,1,1), 0.05f);
		// mainLight.AddComponent<Crimson::DirectionalLightComponent>(glm::vec3(1,1,1), 1.0f);
		//
		// auto pinkLight = m_scene->CreateEntity("Pink light");
		// pinkLight.GetComponent<Crimson::TransformComponent>().position = glm::vec3(-3.5f, 0, 0);
		// pinkLight.AddComponent<Crimson::PointLightComponent>(1.0f, 0.09f, 0.032f, glm::vec3(1,0,1), 1.0f);
		//
		// auto greenLight = m_scene->CreateEntity("Green light");
		// greenLight.GetComponent<Crimson::TransformComponent>().position.x = 3.5f;
		// greenLight.AddComponent<Crimson::PointLightComponent>(1.0f, 0.09f, 0.032f, glm::vec3(0,1,0), 1.0f);
		//
		// auto monkey = m_scene->CreateEntity("Monkey");
		// monkey.GetComponent<Crimson::TransformComponent>().position = glm::vec3(0, 5, 0);
		// monkey.AddComponent<Crimson::BoxColliderComponent>(glm::vec3(1.0f));
		// monkey.AddComponent<Crimson::PhysicsComponent>(true, 1.0f, 0.3f, 0.1f, false, glm::vec3(0));
		// monkey.AddComponent<Crimson::MeshFilterComponent>("Cube");
		// monkey.AddComponent<Crimson::MaterialComponent>("Data/MonkeyMaterial.mat");
		// monkey.AddComponent<Crimson::ScriptComponent>("Monkey");
		//
		// auto monkey2 = m_scene->CreateEntity("Monkey #2");
		// monkey2.GetComponent<Crimson::TransformComponent>().position.x = -5.0f;
		// monkey2.AddComponent<Crimson::MeshFilterComponent>("Data/MonkeyMesh.mesh");
		// monkey2.AddComponent<Crimson::MaterialComponent>("Data/MonkeyMaterial.mat");
		//
		// auto floor = m_scene->CreateEntity("Floor");
		// floor.GetComponent<Crimson::TransformComponent>().scale = glm::vec3(10.0f, 0.5f, 10.0f);
		// floor.GetComponent<Crimson::TransformComponent>().position = glm::vec3(0.0f, -2.0f, 0.0f);
		// floor.AddComponent<Crimson::BoxColliderComponent>(glm::vec3(10.0f, 0.5f, 10.0f));
		// floor.AddComponent<Crimson::PhysicsComponent>(true, 1.0f, 0.3f, 0.1f, true, glm::vec3(0));
		// floor.AddComponent<Crimson::MeshFilterComponent>("Data/CubeMesh.mesh");
		// floor.AddComponent<Crimson::MaterialComponent>("Data/MonkeyMaterial.mat");
		// floor.AddComponent<Crimson::ScriptComponent>("TestBehaviour");
		//
		//
		// cam = m_scene->CreateEntity("Main Camera");
		// cam.GetComponent<Crimson::TransformComponent>().position = glm::vec3(0.0f, 5.0f, 20.0f);
		// cam.GetComponent<Crimson::TransformComponent>().rotation = glm::vec3(-20.0f, 180.0f, 0.0f);
		// cam.AddComponent<Crimson::CameraComponent>(GetWindowSize(), 45.0f).active = true;

		auto s = Crimson::SceneSerialiser(*m_scene);
		//s.SerialiseText("Data/Test.cscn");
		s.DeserialiseText(m_scene->m_assetManager.LoadText("Data/Test.cscn"));

		m_scene->Init();
	}

	void OnUpdate(float delta) override {
		m_camera.UpdateViewport(m_renderTarget->GetSize());

		m_scene->Update(delta);
		m_scene->UpdateViewport(m_renderTarget->GetSize());
		m_scene->Render(*m_renderTarget, m_camera.GetCamera());
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
