#include "Entity.h"

namespace Crimson {
	Entity::Entity(entt::entity entityHandle, Scene* scene)
	 	: m_entityHandle(entityHandle), m_scene(scene) {}
}
