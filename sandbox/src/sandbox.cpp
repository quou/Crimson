#include <crimson.h>

using namespace Crimson;

class Sandbox : public Crimson::Application {
private:
	PBRMaterial m_material;
	ref<Model> m_model;
	float m_pos = 0.0f;
 
	Crimson::Camera camera;
public:
	void OnInit() override {
		m_material = {vec3(1.0f, 0.0f, 0.0f), 1.0, 0.3f};

		m_model = ref<Model>(new Model());
		m_model->AddMesh(MeshFactory::NewCubeMesh());

		camera = Crimson::Camera(m_window->GetWidth(), m_window->GetHeight(), 70.0f, 0.1f, 100.0f);
		camera.position = vec3(0.0f, 0.5f, 5.0f);
	}

	void OnUpdate(float delta) override {
		m_pos += 0.1f;

		Crimson::AssetManager::HotReload();

		Crimson::Renderer::Clear(0.0f, 0.0f, 0.0f);
		
		ref<Shader> s = Crimson::AssetManager::LoadShader("standard.glsl");
		s->Bind();

		camera.projection = mat4::persp(70.0f, (float)m_window->GetWidth()/(float)m_window->GetHeight(), 0.1f, 20.0f);

		mat4 transform = mat4::translate(vec3(0.0f, 0.0f, 0.0f));
		transform *= mat4::rotate(45.0f, vec3(0.0f, 1.0f, 0.0f));

		s->SetUniformMat4("u_model", transform);
		s->SetUniformMat4("u_view", camera.GetView());
		s->SetUniformMat4("u_projection", camera.projection);
		s->SetUniformVec3("u_cameraPos", camera.position);
		
		s->SetUniformVec3("u_cameraPos", camera.position);
		s->SetUniformInt("u_pointLightCount", 1);
		s->SetUniformVec3("u_pointLights[0].position", vec3(2.0f, 0.0f, 5.0f));
		s->SetUniformVec3("u_pointLights[0].color", vec3(1.0f, 1.0f, 1.0f));
		s->SetUniformFloat("u_pointLights[0].intensity", 5.0f);

		m_material.Apply(s);

		m_model->Draw();
	}
};

int main() {
	Sandbox app;
	app.Run("Sandbox", 800, 600);
}
