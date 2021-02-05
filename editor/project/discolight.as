class DiscoLight : CrimsonBehaviour {
	private PointLight@ light;

	private array<vec3> colours;
	private uint colorIndex;
	private float counter;

	float switchDelay;

	void OnInit() {
		entity.GetComponent(@light);

		if (@light == null) {
			LogError("Disco Light requires a point light component");
		}

		colours = {
			vec3(1.0f, 0.0f, 0.0f),
			vec3(0.0f, 1.0f, 0.0f),
			vec3(0.0f, 0.0f, 1.0f),
			vec3(0.0f, 1.0f, 1.0f),
			vec3(1.0f, 1.0f, 0.0f),
		};
	}

	void OnUpdate(float delta) {
		counter += delta;
		if (counter > switchDelay) {
			colorIndex++;
			if (colorIndex >= colours.length() - 1) {
				colorIndex = 0;
			}

			light.color = colours[colorIndex];

			counter = 0.0f;
		}
	}
};