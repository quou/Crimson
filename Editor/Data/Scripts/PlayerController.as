#include "Crimson"

class PlayerController : CrimsonBehaviour {
	float m_moveForce = 20.0f;

	// Called on the first frame
	void OnInit() {
		vec3 test = eulerAngles(GetTransformComponent().rotation);
		quat testQuat(test);
		vec3 finalEuler = eulerAngles(testQuat);
		print(to_string(toDegrees(test)));
		
		
		Console::Log("Test Message");
		Console::LogWarning("Test Warning");
		Console::LogError("Test Error");
		Console::LogFatalError("Test Fatal Error");
	}

	// Called once per frame
	void OnPhysicsUpdate(float delta) {
		vec3 move;
		
		if (Input::GetButton("up").pressed) {
			move.z = m_moveForce;
		} else if (Input::GetButton("down").pressed) {
			move.z = -m_moveForce;
		}
		
		if (Input::GetButton("jump").pressed) {
			move.y = m_moveForce;
		} else if (Input::GetButton("shift").pressed) {
			move.y = -m_moveForce;
		}

		if (Input::GetButton("right").pressed) {
			move.x = -m_moveForce;
		} else if (Input::GetButton("left").pressed) {
			move.x = m_moveForce;
		}
		
		m_entity.GetPhysicsComponent().ApplyForce(move);
	}
};
