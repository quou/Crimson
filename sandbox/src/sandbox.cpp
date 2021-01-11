#include <crimson.h>

using namespace Crimson;

class Sandbox : public Crimson::Application {
private:
	ref<Model> m_model;
	float m_pos = 0.0f;
public:
	void OnInit() override {
		m_model = ref<Model>(new Model());
		m_model->AddMesh(MeshFactory::NewCubeMesh());
	}

	void OnUpdate(float delta) override {
		m_pos += 0.1f;

		Crimson::AssetManager::HotReload();

		Crimson::Renderer::Clear(0.0f, 0.0f, 0.0f);
		
		ref<Shader> s = Crimson::AssetManager::LoadShader("standard.glsl");
		s->Bind();

		mat4 projection = mat4::persp(70.0f, 800.0f/600.0f, 0.1f, 20.0f);

		mat4 model = mat4::translate(vec3(0.0f, 0.0f, -4.0f));
		model *= mat4::rotate(45.0f, vec3(0.0f, 1.0f, 0.0f));

		mat4 view = mat4::translate(vec3(0.0f, -0.5f, 0.0f));

		s->SetUniformMat4("u_model", model);
		s->SetUniformMat4("u_view", view);
		s->SetUniformMat4("u_projection", projection);
		
		m_model->Draw();
	}
};

int main() {
	Sandbox app;
	app.Run("Sandbox", 800, 600);
}
