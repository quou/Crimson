#include "Crimson"

class Monkey : CrimsonBehaviour {
	void OnInit() {
		print("Monkey says hello");
		m_entity.GetTransformComponent().position.x = -5.0f;
	}

	void OnUpdate(float delta) {
		m_entity.GetTransformComponent().position.x += 1.0f * delta;
	}
}
