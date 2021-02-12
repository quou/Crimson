class BackAndForth : CrimsonBehaviour {
	private Transform@ transform;
	private float posOffset;
	private bool hasTransform;

	float speed = 3.0f;

	void OnInit() {
		entity.GetComponent(@transform);

		if (@transform == null) {
			LogError("No transform component");
			hasTransform = false;
		} else {
			hasTransform = true;
		}
	}

	void OnUpdate(float delta) {
		if (!hasTransform) {
			return;
		}

		posOffset += speed * delta;

		transform.translation.x = sin(posOffset) * 5;
	}

};