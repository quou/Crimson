#include <Crimson.h>

class ImGuiLayer : public Crimson::Layer {
public:
	void OnInit() {

	}

	void OnUpdate(float delta) {
		ImGui::Begin("Test Window");
		ImGui::End();
	}
};

class Game : public Crimson::Game {
public:
	Crimson::AssetManager m_assetManager;

	std::shared_ptr<Crimson::Shader> m_shader;
	std::shared_ptr<Crimson::Mesh> m_mesh;
private:
	void OnInit() override {
		AddLayer<ImGuiLayer>();

		m_shader = std::make_shared<Crimson::Shader>(m_assetManager.LoadText("Data/Standard.glsl"));
		m_mesh = std::make_shared<Crimson::Mesh>(
			std::vector<Crimson::Vertex> {
				Crimson::Vertex{glm::vec3(-1,-1,-1), glm::vec3(0, 0, -1), glm::vec2(1, 0)},
				Crimson::Vertex{glm::vec3(-1,1,-1), glm::vec3(0, 0, -1), glm::vec2(1, 0)},
				Crimson::Vertex{glm::vec3(1,1,-1), glm::vec3(0, 0, -1), glm::vec2(1, 0)},
				Crimson::Vertex{glm::vec3(1,-1,-1), glm::vec3(0, 0, -1), glm::vec2(1, 0)},
			},
			std::vector<unsigned int> {
				0, 1, 2,
				0, 2, 3,
			}
		);
	}

	void OnUpdate(float delta) override {
		m_renderer->DrawMesh(m_shader, m_mesh);
	}

	void OnExit() override {
		m_shader.reset();
		m_mesh.reset();
	}
public:
};

int main(void) {
	Game game;
	game.Run("Game", {1366, 768});
}
