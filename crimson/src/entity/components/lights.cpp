#include "lights.h"
#include "entity/scene.h"
#include "transform.h"

namespace Crimson {
	void PointLightComponent::OnInit() {
		if (!m_entity->HasComponent<TransformComponent>()) {
			Log(LogType::WARNING, "Point Light requires the entity to have a transform component");
		}

		m_entity->m_scene->GetPointLights()->push_back(this);
	}

	void PointLightComponent::OnDestroy() {
		m_entity->m_scene->RemovePointLight(this);
	}


	const vec3& PointLightComponent::GetPosition() {
		if (!m_entity->HasComponent<TransformComponent>()) {
			return vec3();
		}

		return m_entity->GetComponent<TransformComponent>()->translation;
	}

	void SkyLightComponent::OnInit() {
		m_entity->m_scene->GetSkyLights()->push_back(this);
	}

	void SkyLightComponent::OnDestroy() {
		m_entity->m_scene->RemoveSkyLight(this);
	}
}