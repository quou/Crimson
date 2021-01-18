class TestBehaviour : CrimsonBehaviour {
	void OnInit() {
		dictionary dict = {{"thing", 12}, {"somestring", "hello, there"}, {"haha", 100}};

		print(string(dict["somestring"]));
	}

	void OnUpdate(float delta) {
		
	}
}
