#include <Crimson.h>

class ImGuiLayer : public Crimson::Layer {
public:
	void OnInit() {
		ImGui::GetIO().IniFilename = "";
	}

	void OnUpdate(float delta) {

	}
};

class Game : public Crimson::Game {
public:
	std::shared_ptr<Crimson::Scene> m_scene;
private:
	void OnInit() override {
		AddLayer<ImGuiLayer>(NULL);

		m_scene = std::make_shared<Crimson::Scene>(true);

		auto s = Crimson::SceneSerialiser(*m_scene);
		s.DeserialiseText(m_scene->m_assetManager.LoadText("Data/Test.cscn"));

		m_scene->Init();
	}

	void OnUpdate(float delta) override {
		m_scene->Update(delta);
		m_scene->UpdateViewport(GetWindowSize());
		m_scene->Render();
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
