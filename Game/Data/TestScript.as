#include "Crimson"

class TestBehaviour : CrimsonBehaviour {
	void OnInit() {
		print("Hello, world");
	}

	void OnUpdate(float delta) {
		print(to_string(delta));
	}
};
