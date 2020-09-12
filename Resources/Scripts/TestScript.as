int counter = 0;

void OnBegin() {
	print("Hello from AngelScript");
}

void OnUpdate(float delta) {
	counter++;

	if (counter > 5000 * delta) {
		print("Hello from the update loop 2");
		print("Hello from the update loop");
		counter = 0;
	}
}
