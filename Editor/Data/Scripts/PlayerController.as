class PlayerController : CrimsonBehaviour {
	private float m_moveForce = 20.0f;
	
	private float m_counter = 0.0f;

	private string m_greenMaterial = "Data/Materials/GreenMaterial.mat";
	private string m_redMaterial = "Data/Materials/RedMaterial.mat";
	
	// Called on the first frame
	void OnInit() {
		Console::Log("Test Message");
		Console::LogWarning("Test Warning");
		Console::LogError("Test Error");
		Console::LogFatalError("Test Fatal Error");

		m_entity.GetMaterialComponent().resource = m_greenMaterial;

		Console::Log(to_string(KEY_SPACE));
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
		
		m_counter += delta;
		if (m_counter > 1.0f) {
			if (m_entity.GetMaterialComponent().resource == m_greenMaterial) {
				m_entity.GetMaterialComponent().resource = m_redMaterial;
			} else {
				m_entity.GetMaterialComponent().resource = m_greenMaterial;
			}
			m_counter = 0.0f;
		}
	}
};
