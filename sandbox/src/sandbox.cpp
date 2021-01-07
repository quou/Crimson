#include <crimson.h>

class Sandbox : public Crimson::Application {
public:
	void OnInit() override {
		std::string e = Crimson::AssetManager::LoadTerminatedString("standard.glsl");
		Crimson::Log(Crimson::LogType::INFO, "%s", e.c_str());

		Crimson::Shader::ShaderSource s = Crimson::Shader::Parse(e.c_str());	
		Crimson::Shader shader(s.vertex.c_str(), s.pixel.c_str());

		for (auto& u : shader.GetUniforms()) {
			Crimson::Log(Crimson::LogType::INFO, "%s", u.name.c_str());
		}
	}

	void OnUpdate(float delta) override {
		Crimson::AssetManager::HotReload();

		Crimson::Renderer::Clear(1.0f, 1.0f, 1.0f);
	}
};

int main() {
	Sandbox app;
	app.Run("Sandbox", 800, 600);
}
