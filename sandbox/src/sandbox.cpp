#include <crimson.h>

using namespace Crimson;

class Sandbox : public Crimson::Application {
private:
	ref<Mesh> m_mesh;
	float m_pos = 0.0f;
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
		m_pos += 0.001f;

		Crimson::AssetManager::HotReload();

		Crimson::Renderer::Clear(0.0f, 0.0f, 0.0f);
		
		ref<Shader> s = Crimson::AssetManager::LoadShader("standard.glsl");
		s->Bind();

		mat4 projection = mat4::persp(70.0f, 800.0f/600.0f, 0.1f, 20.0f);

		mat4 model = mat4::translate(vec3(0.0f, 0.0f, -2.0f));

		s->SetUniformMat4("u_model", model);
		s->SetUniformMat4("u_projection", projection);
		
		m_mesh->Draw();
	}
};

int main() {
	Sandbox app;
	app.Run("Sandbox", 800, 600);
}
