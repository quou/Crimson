#include <crimson.h>
#include <imgui.h>

using namespace Crimson;

class Sandbox : public Crimson::Application {
private:
	float m_pos = 0.0f;

	ref<Scene> m_scene;
	Entity* ent;
	Entity* pointLight;
	Entity* pointLight2;
	
	Crimson::Camera camera;
public:
	void OnInit() override {
		ImGuiManager::Init(m_window);

		m_scene = ref<Scene>(new Scene());

		ref<Material> material(new PBRMaterial("standard.glsl", vec3(1.0f, 1.0f, 1.0f), 1.0, 0.3f));

		ref<Model> model = ref<Model>(new Model());
		model->AddMesh(MeshFactory::NewCubeMesh(material));

		ent = m_scene->CreateEntity();
		ent->AddComponent<TransformComponent>()->Rotate(45.0f, vec3(0.0f, 1.0f, 0.0f));
		ent->AddComponent<RenderableComponent>(model);

		pointLight = m_scene->CreateEntity();
		pointLight->AddComponent<TransformComponent>()->Translate(vec3(2.0f, -1.0f, 5.0f));
		pointLight->AddComponent<PointLightComponent>(vec3(1.0f), 1.0f);

		pointLight2 = m_scene->CreateEntity();
		pointLight2->AddComponent<TransformComponent>()->Translate(vec3(2.0f, 0.0f, 5.0f));
		pointLight2->AddComponent<PointLightComponent>(vec3(1.0f, 0.0f, 0.0f), 3.0f);

		camera = Crimson::Camera(m_window->GetWidth(), m_window->GetHeight(), 70.0f, 0.1f, 100.0f);
		camera.position = vec3(0.0f, 0.5f, 5.0f);
	}

	void OnUpdate(float delta) override {
		m_scene->Update(delta);
		Crimson::AssetManager::HotReload();
		Crimson::Renderer::Clear(0.0f, 0.0f, 0.0f);

		camera.projection = mat4::persp(70.0f, (float)m_window->GetWidth()/(float)m_window->GetHeight(), 0.1f, 20.0f);

		m_scene->Draw(camera);

		ImGuiManager::BeginFrame();

		ImGui::Begin("Test Window");

		ImGui::End();

		ImGuiManager::EndFrame();
	}

	void OnExit() {
		ImGuiManager::Quit();
	}
};

int main() {
	Sandbox app;
	app.Run("Sandbox", 800, 600);
}
