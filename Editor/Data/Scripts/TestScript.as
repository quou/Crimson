#include "Crimson"

class TestBehaviour : CrimsonBehaviour {
	private vec3 m_testVec(1, 3, 4);

	void OnInit() {
		print(to_string(m_testVec));
		m_entity.GetTransformComponent().tag = "Cube";
		m_entity.GetPhysicsComponent().rigidbody.SetBounciness(0.1f);
		m_entity.GetPhysicsComponent().rigidbody.SetFriction(1.0f);
	}

	void OnUpdate(float delta) {
		//print(to_string(delta));
	}

	void OnContactEnter(Entity other) {
		print("Cube entered " + other.GetTransformComponent().tag);
	}
};
