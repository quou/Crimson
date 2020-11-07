#include "Crimson"

class Monkey : CrimsonBehaviour {
	private float m_moveForce = 20.0f;

	void OnInit() {
		print("Monkey says hello");

		m_entity.GetPhysicsComponent().rigidbody.EnableGravity(true);
		m_entity.GetPhysicsComponent().rigidbody.SetBounciness(0.1f);
		m_entity.GetPhysicsComponent().rigidbody.SetFriction(1.0f);
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

	void OnContactEnter(Entity other) {
		print(m_entity.GetTransformComponent().name + " entered " + other.GetTransformComponent().name);
	}
}
