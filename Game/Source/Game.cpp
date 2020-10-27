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
	Crimson::AssetManager m_assetManager;

	std::shared_ptr<Crimson::Scene> m_scene;
private:
	void OnInit() override {
		AddLayer<ImGuiLayer>();

		m_scene = std::make_shared<Crimson::Scene>();
		m_scene->m_lightScene->m_ambientLights.push_back({glm::vec3(1,1,1), 0.1f});
		m_scene->m_lightScene->m_directionalLights.push_back({glm::vec3(-1,-1,-1), glm::vec3(1,1,1), 1.0f});

		Crimson::Entity testEnt = m_scene->CreateEntity();
		testEnt.GetComponent<Crimson::TransformComponent>().rotation.y = 45.0f;
		testEnt.AddComponent<Crimson::MeshFilterComponent>(m_assetManager.LoadText("Data/MonkeyMesh.mesh").c_str());
		testEnt.AddComponent<Crimson::MaterialComponent>(m_assetManager.LoadText("Data/MonkeyMaterial.mat"), m_assetManager);

		Crimson::Entity cam = m_scene->CreateEntity();
		cam.GetComponent<Crimson::TransformComponent>().position = glm::vec3(0, 0, -5);
		cam.AddComponent<Crimson::CameraComponent>(GetWindowSize(), 45.0f).active = true;
	}

	void OnUpdate(float delta) override {
		m_scene->Update(delta);
		m_scene->UpdateViewport(GetWindowSize());

		ImGui::Begin("hi");
		ImGui::End();
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
