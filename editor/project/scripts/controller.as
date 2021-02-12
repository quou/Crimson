class Controller : CrimsonBehaviour {
	private Transform@ transform;
	private bool panicMode = false;

	float speed;

	void OnInit() {
		entity.GetComponent(@transform);
		if (@transform == null) {
			panicMode = true;
		}
	}

	void OnUpdate(float delta) {
		if (panicMode) { return; }

		if (EventSystem::KeyPressed(KEY_LEFT)) {
			transform.translation.x += speed * delta;
		} else if (EventSystem::KeyPressed(KEY_RIGHT)) {
			transform.translation.x -= speed * delta;
		}

		if (EventSystem::KeyPressed(KEY_UP)) {
			transform.translation.z += speed * delta;
		} else if (EventSystem::KeyPressed(KEY_DOWN)) {
			transform.translation.z -= speed * delta;
		}
	}
}