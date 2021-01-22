class TestBehaviour : CrimsonBehaviour {
	private Transform@ m_transform;

	void OnInit() {
		entity.GetComponent(@m_transform);
		m_transform.Translate(vec3(3.0f, 0.0f, 0.0f));
	}

	void OnUpdate(float delta) {
		
	}
}
