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

	std::shared_ptr<Crimson::Material> m_material;
	std::shared_ptr<Crimson::Texture> m_texture;
	std::shared_ptr<Crimson::Mesh> m_mesh;

	Crimson::Transform m_transform;
private:
	void OnInit() override {
		AddLayer<ImGuiLayer>();

		m_material = std::make_shared<Crimson::Material>(m_assetManager.LoadText("Data/MonkeyMaterial.lua"), m_assetManager);
		m_texture = std::make_shared<Crimson::Texture>(m_assetManager.LoadSurface("Data/Wood.jpg"));

		m_mesh = std::make_shared<Crimson::Mesh>(m_assetManager.LoadText("Data/Monkey.obj").c_str());

		m_transform.position = glm::vec3(0, 0, -5);
		m_transform.rotation = glm::vec3(0, 45, 0);
		m_transform.scale = glm::vec3(1, 1, 1);
	}

	void OnUpdate(float delta) override {
		m_texture->Bind(0);
		m_renderer->Draw(m_transform, m_material->m_shader, m_mesh);

		ImGui::Begin("Transform");
		ImGui::InputFloat("Rotation X", &m_transform.rotation.x);
		ImGui::InputFloat("Rotation Y", &m_transform.rotation.y);
		ImGui::InputFloat("Rotation Z", &m_transform.rotation.z);

		static float newscale = 1.0f;
		ImGui::InputFloat("Scale", &newscale);
		m_transform.scale = glm::vec3(newscale);

		ImGui::End();
	}

	void OnExit() override {
		m_texture.reset();
		m_material.reset();
		m_mesh.reset();
	}
public:
};

int main(void) {
	Game game;
	game.Run("Game", {1366, 768});
}
