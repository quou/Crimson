class TestBehaviour : CrimsonBehaviour {
	private vec3 m_testVec(1, 3, 4);

	void OnInit() {
		print(to_string(m_testVec));
	}

	void OnUpdate(float delta) {
		//print(to_string(delta));
	}
};
