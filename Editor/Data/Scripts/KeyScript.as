#include "Crimson"

class KeyScript : CrimsonBehaviour {
	void OnInit() {

	}

	void OnUpdate(float delta) {
		vec3 move(0);

		if (Input::GetButton("up").pressed) {
			move.y = 3.0f * delta;
		} else if (Input::GetButton("down").pressed) {
			move.y = -3.0f * delta;
		}

		if (Input::GetButton("a").pressed) {
			move.x = 3.0f * delta;
		} else if (Input::GetButton("d").pressed) {
			move.x = -3.0f * delta;
		}

		if (Input::GetButton("w").pressed) {
			move.z = 3.0f * delta;
		} else if (Input::GetButton("s").pressed) {
			move.z = -3.0f * delta;
		}

		m_entity.GetTransformComponent().position += move;
	}
};
