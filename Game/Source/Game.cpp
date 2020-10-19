#include <Crimson.h>

class Game : public Crimson::Game {
private:
	void OnInit() override {

	}

	void OnUpdate(float delta) override {

	}
public:
};

int main(void) {
	Game game;
	game.Run("Game", {1366, 768});
}
