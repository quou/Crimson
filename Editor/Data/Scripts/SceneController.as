#include "Crimson"

class SceneController : CrimsonBehaviour {
	private float m_count;
	private int m_iteration;

	// Called on the first frame
	void OnInit() {
		
	}

	// Called once per frame
	void OnUpdate(float delta) {
		m_count += delta;

		if (m_count > 1) {
			m_iteration++;
			print("Iteration: " + to_string(m_iteration));
			m_count = 0.0f;
		}
	}
};
