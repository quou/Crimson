class TestBehaviour : CrimsonBehaviour {
	private Transform@ m_transform;
	private float m_posOffset;

	int someInt = 10;
	float someFloat = 22.2;
	string someString = "hello, world";

	void OnInit() {
		entity.GetComponent(@m_transform);

		print(someString);
		print(ToString(someFloat));
		print(ToString(someInt));
	}

	void OnUpdate(float delta) {
		m_posOffset += 3.0f * delta;

		m_transform.translation.x = sin(m_posOffset) * 5;
	}
};


class DiscoLight : CrimsonBehaviour {
	private PointLight@ m_light;

	private array<vec3> m_colours;
	private uint m_colorIndex;
	private float m_counter;

	void OnInit() {
		entity.GetComponent(@m_light);

		if (@m_light == null) {
			LogError("Disco Light requires a point light component");
		}

		m_colours = {
			vec3(1.0f, 0.0f, 0.0f),
			vec3(0.0f, 1.0f, 0.0f),
			vec3(0.0f, 0.0f, 1.0f),
			vec3(0.0f, 1.0f, 1.0f),
			vec3(1.0f, 1.0f, 0.0f),
		};
	}

	void OnUpdate(float delta) {
		m_counter += delta;
		if (m_counter > 0.5f) {
			m_colorIndex++;
			if (m_colorIndex >= m_colours.length() - 1) {
				m_colorIndex = 0;
			}

			m_light.color = m_colours[m_colorIndex];

			m_counter = 0.0f;
		}
	}
};