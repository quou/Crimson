#include "Entity.h"

namespace Crimson {
	Entity::Entity(entt::entity entityHandle, Scene* scene)
	 	: m_entityHandle(entityHandle), m_scene(scene) {}

	void Entity::Destroy() {
		m_scene->m_registry.destroy(m_entityHandle);
	}

	bool Entity::IsValid() {
		return m_scene->m_registry.valid(m_entityHandle);
	}
}
