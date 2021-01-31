class Sphere : CrimsonBehaviour {
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