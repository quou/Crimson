class TestBehaviour : CrimsonBehaviour {
	private float m_counter = 0.0f;

	void OnInit() {
		
	}

	void OnUpdate(float delta) {
		m_counter += delta;
		if (m_counter > 1.0f) {
			entity.Destroy();
		}
	}
}
