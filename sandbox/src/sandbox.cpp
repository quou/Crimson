#include <crimson.h>

using namespace Crimson;

class Sandbox : public Crimson::Application {
private:
	float m_pos = 0.0f;

	ref<Scene> m_scene;
	Entity* ent;
	
	Crimson::Camera camera;
public:
	void OnInit() override {
		m_scene = ref<Scene>(new Scene());

		ref<Material> material(new PBRMaterial(vec3(1.0f, 1.0f, 1.0f), 1.0, 0.3f));

		ref<Model> model = ref<Model>(new Model());
		model->AddMesh(MeshFactory::NewCubeMesh());

		ent = m_scene->CreateEntity();
		ent->AddComponent<TransformComponent>().Rotate(45.0f, vec3(0.0f, 1.0f, 0.0f));
		ent->AddComponent<RenderableComponent>(model, material, "standard.glsl");

		camera = Crimson::Camera(m_window->GetWidth(), m_window->GetHeight(), 70.0f, 0.1f, 100.0f);
		camera.position = vec3(0.0f, 0.5f, 5.0f);
	}

	void OnUpdate(float delta) override {
		m_scene->Update(delta);
		Crimson::AssetManager::HotReload();
		Crimson::Renderer::Clear(0.0f, 0.0f, 0.0f);

		camera.projection = mat4::persp(70.0f, (float)m_window->GetWidth()/(float)m_window->GetHeight(), 0.1f, 20.0f);

		m_scene->Draw(camera);
	}
};

int main() {
	Sandbox app;
	app.Run("Sandbox", 800, 600);
}
