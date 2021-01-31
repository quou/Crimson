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