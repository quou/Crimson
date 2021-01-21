#include <crimson.h>
#include <imgui.h>

class Sandbox : public Crimson::Application {
private:
	Crimson::ref<Crimson::Scene> m_scene;
	Crimson::Entity* ent;
	Crimson::Entity* pointLight;
	Crimson::Entity* pointLight2;

	float m_rot;
	
	Crimson::Camera m_camera;
public:
	void OnInit() override {
		Crimson::ImGuiManager::Init(m_window);

		/* Create the scene */
		m_scene = Crimson::ref<Crimson::Scene>(new Crimson::Scene());

		/* Create a sphere model */
		Crimson::ref<Crimson::Material> material(new Crimson::PhongMaterial("standard.glsl", Crimson::vec3(0.7f, 0.0f, 0.1f)));
		Crimson::ref<Crimson::Model> model(new Crimson::Model());
		model->AddMesh(Crimson::MeshFactory::NewSphereMesh(material));

		/* Create the sphere entity */
		ent = m_scene->CreateEntity();
		ent->AddComponent<Crimson::TransformComponent>();
		ent->AddComponent<Crimson::RenderableComponent>(model);

		/* Create point light entities */
		pointLight = m_scene->CreateEntity();
		pointLight->AddComponent<Crimson::TransformComponent>()->Translate(Crimson::vec3(5.0f, -1.0f, 4.0f));
		pointLight->AddComponent<Crimson::PointLightComponent>(Crimson::vec3(1.0f), 20.0f);

		/* Create the camera */
		m_camera = Crimson::Camera(m_window->GetWidth(), m_window->GetHeight(), 70.0f, 0.1f, 100.0f);
		m_camera.position = Crimson::vec3(0.0f, 0.5f, 5.0f);
	}

	void OnUpdate(float delta) override {
		m_rot += 0.001f * delta;
		if (m_rot >= 360) {
			m_rot = 0.0f;
		}

		ent->GetComponent<Crimson::TransformComponent>()->Rotate(m_rot, Crimson::vec3(0.0f, 1.0f, 0.0f));

		m_scene->Update(delta);
		Crimson::AssetManager::HotReload();

		Crimson::Renderer::Clear(0.0f, 0.0f, 0.0f);

		/* Update perspective */
		m_camera.projection = Crimson::mat4::persp(70.0f, (float)m_window->GetWidth()/(float)m_window->GetHeight(), 0.1f, 20.0f);

		m_scene->Draw(m_camera);

		Crimson::ImGuiManager::BeginFrame();

		ImGui::Begin("Test Window");
		ImGui::Text("%g", 1.0f/delta);
		ImGui::End();

		Crimson::ImGuiManager::EndFrame();
	}

	void OnExit() {
		Crimson::ImGuiManager::Quit();
	}
};

int main() {
	Sandbox app;
	app.Run("Sandbox", 800, 600);
}
