# Scripting

Crimson uses AngelScript for powerful and fast scripting

Example script:
```cpp
#include "Crimson"

class ExampleBehaviour : CrimsonBehaviour {
	private int m_counter;

	void OnInit() {
		print("Hello, there");
	}

	void OnUpdate(float delta) {
		m_entity.GetTransformComponent().position.x += 2.0f * delta;

		print(to_string(m_counter));
		m_counter++;
	}
}

```
