#pragma once

#include <entt.hpp>

#include "Scene.h"

#include "Logger.h"

namespace Crimson {
	class Entity {
	private:
		entt::entity m_entityHandle;
		Scene* m_scene;
	public:
		Entity(entt::entity entityHandle, Scene* scene);
		Entity(const Entity& other) = default;

		template <typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			CR_ASSERT(!HasComponent<T>(), "%s", "Entity already has component");

			return m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
		}

		template <typename T>
		void RemoveComponent() {
			CR_ASSERT(HasComponent<T>(), "%s", "Entity does not have component");

			m_scene->m_registry.remove<T>(m_entityHandle);
		}

		template <typename T>
		T& GetComponent() {
			CR_ASSERT(HasComponent<T>(), "%s", "Entity does not have component");

			return m_scene->m_registry.get<T>(m_entityHandle);
		}

		template <typename T>
		bool HasComponent() {
			return m_scene->m_registry.has<T>(m_entityHandle);
		}
	};
}
