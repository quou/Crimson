# Scripting Introduction

Crimson uses AngelScript for powerful and fast scripting

Example script:
```cpp
#include "Crimson"

class ExampleBehaviour : CrimsonBehaviour {
	private float m_counter;

	// Called before the first frame
	void OnInit() {
		print("Hello, there");
	}

	// Called once per frame
	void OnUpdate(float delta) {
		m_entity.GetTransformComponent().position.x += 2.0f * delta;

		m_counter += delta;
		if (m_counter > 10) {
			Destroy(); // Simply a wrapper around m_entity.Destroy()
		}
	}
}

```
