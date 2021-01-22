class TestBehaviour : CrimsonBehaviour {
	private Transform@ m_transform;
	private float m_posOffset;
	private float m_scaleOffset;

	private float Clamp(float v, float lo, float hi) {
		return (v < lo) ? lo : (hi < v) ? hi : v;
	}

	void OnInit() {
		entity.GetComponent(@m_transform);
	}

	void OnUpdate(float delta) {
		m_posOffset += 3.0f * delta;
		m_scaleOffset += delta;

		m_transform.translation.x = sin(m_posOffset);
		m_transform.scale = vec3(Clamp(sin(m_scaleOffset), 0.1f, 2.0f));
	}
}
