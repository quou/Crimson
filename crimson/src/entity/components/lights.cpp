#include "lights.h"
#include "entity/scene.h"
#include "transform.h"

namespace Crimson {
	void PointLightComponent::OnInit() {
		if (!m_entity->HasComponent<TransformComponent>()) {
			Log(LogType::WARNING, "Point Light requires the entity to have a transform component");
		}

		m_entity->m_scene->GetLights()->push_back(m_entity);
	}

	void PointLightComponent::OnDestroy() {
		m_entity->m_scene->RemoveLight(m_entity);
	}

	void SkyLightComponent::OnInit() {
		m_entity->m_scene->GetLights()->push_back(m_entity);
	}

	void SkyLightComponent::OnDestroy() {
		m_entity->m_scene->RemoveLight(m_entity);
	}
}