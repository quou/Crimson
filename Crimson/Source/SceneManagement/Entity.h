#pragma once

#include <entt.hpp>

#include "Scene.h"

#include "Logger.h"

namespace Crimson {
	class Entity {
	private:
		entt::entity m_entityHandle;
		Scene* m_scene;

		friend class Scene;
	public:
		Entity();
		Entity(entt::entity entityHandle, Scene* scene);
		Entity(const Entity& other) = default;

		void Destroy();
		bool IsValid();

		template <typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			if (HasComponent<T>()) {
				CR_LOG_WARNING("%s", "Entity already has component");
				return GetComponent<T>();
			}

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

		operator bool() const { return m_entityHandle != entt::null; }
		operator uint32_t() const { return (uint32_t)m_entityHandle; }

		bool operator==(const Entity& other) const {
			return m_entityHandle == other.m_entityHandle && m_scene == other.m_scene;
		}

		bool operator!=(const Entity& other) const {
			return m_entityHandle != other.m_entityHandle || m_scene != other.m_scene;
		}
	};
}
