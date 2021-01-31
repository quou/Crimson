class Sphere : CrimsonBehaviour {
	private Transform@ transform;
	private float posOffset;

	int someInt = 10;
	float someFloat = 22.2;
	string someString = "hello, world";

	void OnInit() {
		entity.GetComponent(@transform);

		print(someString);
		print(ToString(someFloat));
		print(ToString(someInt));
	}

	void OnUpdate(float delta) {
		posOffset += 3.0f * delta;

		transform.translation.x = sin(posOffset) * 5;
	}
};