#include <crimson.h>

using namespace Crimson;

class Sandbox : public Crimson::Application {
private:
	ref<Mesh> m_mesh;
public:
	void OnInit() override {
		m_mesh = ref<Mesh>(new Mesh({
			Vertex{vec3(0.5f, 0.5f, 0.0f), vec3(0.0f), vec2(0.0f)},
			Vertex{vec3(0.5f, -0.5f, 0.0f), vec3(0.0f), vec2(0.0f)},
			Vertex{vec3(-0.5f, -0.5f, 0.0f), vec3(0.0f), vec2(0.0f)},
			Vertex{vec3(-0.5f, 0.5f, 0.0f), vec3(0.0f), vec2(0.0f)}},
			{0, 1, 3, 1, 2, 3}));
	}

	void OnUpdate(float delta) override {
		Crimson::AssetManager::HotReload();

		Crimson::Renderer::Clear(0.0f, 0.0f, 0.0f);
		
		Crimson::AssetManager::LoadShader("standard.glsl")->Bind();
		m_mesh->Draw();	
	}
};

int main() {
	Sandbox app;
	app.Run("Sandbox", 800, 600);
}
