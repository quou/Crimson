#include <Crimson.h>

class ImGuiLayer : public Crimson::Layer {
public:
	void OnInit() {

	}

	void OnUpdate(float delta) {
		ImGui::Begin("Test Window");
		ImGui::End();
	}
};

class Game : public Crimson::Game {
private:
	void OnInit() override {
		AddLayer<ImGuiLayer>();

		Crimson::AssetManager assetManager;
		CR_PRINTF("%s\n", assetManager.LoadText("Source/Game.cpp").c_str());
	}

	void OnUpdate(float delta) override {

	}
public:
};

int main(void) {
	Game game;
	game.Run("Game", {1366, 768});
}
