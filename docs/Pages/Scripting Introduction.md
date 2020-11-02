# Scripting Introduction

Crimson uses AngelScript for powerful and fast scripting

Example script:
```cpp
#include "Crimson"

class Monkey : CrimsonBehaviour {
	private float m_speed = 3.0f;

	// Called before the first frame
	void OnInit() {
		print("Monkey says hello");

		m_entity.GetTransformComponent().tag = "Monkey";
	}

	// Called once per frame
	void OnUpdate(float delta) {
		if (Input::GetButton("up").pressed) {
			m_entity.GetTransformComponent().position.y += m_speed * delta;
		} else if (Input::GetButton("down").pressed) {
			m_entity.GetTransformComponent().position.y -= m_speed * delta;
		}

		if (Input::GetButton("right").pressed) {
			m_entity.GetTransformComponent().position.x += m_speed * delta;
		} else if (Input::GetButton("left").pressed) {
			m_entity.GetTransformComponent().position.x -= m_speed * delta;
		}
	}

	// Called when there is a collision
	void OnContact(Entity other) {
		print("[Monkey]" + other.GetTransformComponent().tag);
	}
}
```
