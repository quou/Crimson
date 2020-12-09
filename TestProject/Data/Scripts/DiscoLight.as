#include "Crimson"

class DiscoLight : CrimsonBehaviour {
	float changeRate = 2.0f;

	// Called on the first frame
	void OnInit() {

	}

	// Called once per framea
	void OnUpdate(float delta) {
		m_entity.GetPointLightComponent().color.x += changeRate * delta;
		if (m_entity.GetPointLightComponent().color.x >= 1.0f || m_entity.GetPointLightComponent().color.x <= 0.0f) {
			changeRate = -changeRate;
		}
		
		m_entity.GetPointLightComponent().color.y += changeRate * delta;
		if (m_entity.GetPointLightComponent().color.y >= 1.0f || m_entity.GetPointLightComponent().color.y <= 0.0f) {
			changeRate = -changeRate;
		}
		
		m_entity.GetPointLightComponent().color.z += changeRate * delta;
		if (m_entity.GetPointLightComponent().color.z >= 1.0f || m_entity.GetPointLightComponent().color.z <= 0.0f) {
			changeRate = -changeRate;
		}
	}
};
