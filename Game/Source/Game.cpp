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
	Crimson::Camera m_camera;
	Crimson::LightScene m_lightScene;
private:
	void OnInit() override {
		AddLayer<ImGuiLayer>();

		//Crimson::ConvertFromObj("Data/Monkey.obj", "Data/MonkeyMesh.lua");

		m_material = std::make_shared<Crimson::Material>(m_assetManager.LoadText("Data/MonkeyMaterial.lua"), m_assetManager);
		m_texture = std::make_shared<Crimson::Texture>(m_assetManager.LoadSurface("Data/Wood.jpg"));

		m_mesh = std::make_shared<Crimson::Mesh>(m_assetManager.LoadText("Data/MonkeyMesh.lua").c_str());

		m_camera = Crimson::Camera(GetWindowSize(), 45.0f);
		m_camera.position = glm::vec3(0, 0, -5);

		m_transform.position = glm::vec3(0, 0, 0);
		m_transform.rotation = glm::vec3(0, 45, 0);
		m_transform.scale = glm::vec3(1, 1, 1);

		m_lightScene.m_ambientLights.push_back({glm::vec3(1,1,1), 0.1f});
		m_lightScene.m_directionalLights.push_back({glm::vec3(-1,-1,-1), glm::vec3(1,1,1), 1.0f});
		m_lightScene.m_directionalLights.push_back({glm::vec3(1,1,1), glm::vec3(1,0,0), 0.4f});

		m_lightScene.m_pointLights.push_back({glm::vec3(2.0f, 0, 2.5f), 1.0f, 0.09f, 0.032f, glm::vec3(0, 1, 0), 1.0f});
	}

	void OnUpdate(float delta) override {
		m_texture->Bind(0);
		m_lightScene.Apply(m_camera, m_material->m_shader);
		m_renderer->Draw(m_camera, m_transform, m_material->m_shader, m_mesh);

		ImGui::Begin("Test window");
		ImGui::Text("Monkey");
		ImGui::DragFloat("Rotation X", &m_transform.rotation.x);
		ImGui::DragFloat("Rotation Y", &m_transform.rotation.y);
		ImGui::DragFloat("Rotation Z", &m_transform.rotation.z);

		static float newscale = 1.0f;
		ImGui::DragFloat("Scale", &newscale);
		m_transform.scale = glm::vec3(newscale);

		ImGui::Separator();
		ImGui::Text("Camera");
		ImGui::DragFloat("Position X", &m_camera.position.x);
		ImGui::DragFloat("Position Y", &m_camera.position.y);
		ImGui::DragFloat("Position Z", &m_camera.position.z);

		ImGui::DragFloat("Camera Rotation X", &m_camera.rotation.x);
		ImGui::DragFloat("Camera Rotation Y", &m_camera.rotation.y);
		ImGui::DragFloat("Camera Rotation Z", &m_camera.rotation.z);

		ImGui::Separator();
		ImGui::Text("Light");
		ImGui::DragFloat("Direction X", &m_lightScene.m_pointLights[0].position.x, 0.05f, -100.0f, 100.0f);
		ImGui::DragFloat("Direction Y", &m_lightScene.m_pointLights[0].position.y, 0.05f, -100.0f, 100.0f);
		ImGui::DragFloat("Direction Z", &m_lightScene.m_pointLights[0].position.z, 0.05f, -100.0f, 100.0f);

		ImGui::End();

		m_camera.UpdateProjection(GetWindowSize(), 45.0f);
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
