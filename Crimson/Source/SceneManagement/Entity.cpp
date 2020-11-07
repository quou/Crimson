#include "Entity.h"

namespace Crimson {
	Entity::Entity(entt::entity entityHandle, Scene* scene)
	 	: m_entityHandle(entityHandle), m_scene(scene) {}

	Entity::Entity() {
		m_entityHandle = entt::null;
		m_scene = NULL;
	}

	void Entity::Destroy() {
		m_scene->m_registry.destroy(m_entityHandle);
	}

	bool Entity::IsValid() {
		return m_scene->m_registry.valid(m_entityHandle);
	}
}
