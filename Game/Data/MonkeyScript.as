#include "Crimson"

class Monkey : CrimsonBehaviour {
	private float m_moveForce = 20.0f;

	void OnInit() {
		print("Monkey says hello");

		m_entity.GetTransformComponent().tag = "Monkey";
	}

	void OnUpdate(float delta) {
		vec3 move(0);

		if (Input::GetButton("up").pressed) {
			move.y = m_moveForce;
		} else if (Input::GetButton("down").pressed) {
			move.y = -m_moveForce;
		}

		if (Input::GetButton("right").pressed) {
			move.x = m_moveForce;
		} else if (Input::GetButton("left").pressed) {
			move.x = -m_moveForce;
		}

		m_entity.GetPhysicsComponent().rigidbody.ApplyForce(move);
	}

	void OnContact(Entity other) {
		print("[Monkey]" + other.GetTransformComponent().tag);
	}
}
