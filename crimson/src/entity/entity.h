#pragma once

#include <stddef.h>
#include <stdint.h>

#include <vector>
#include <algorithm>
#include <bitset>
#include <array>
#include <cassert>
#include <type_traits>

#include "core.h"
#include "memory.h"
#include "logger.h"
#include "rendering/camera.h"

namespace Crimson {
	class Entity;
	class Component;
	class Scene;

	using ComponentID = size_t;

	ComponentID CR_API GetUniqueComponentID() noexcept;

	template <typename T>
	inline ComponentID GetComponentTypeID() noexcept {
		static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

		static ComponentID typeID{ GetUniqueComponentID() };
		return typeID;
	}

	constexpr size_t maxComponents{32};

	using ComponentBitset = std::bitset<maxComponents>;
	using ComponentArray = std::array<Component*, maxComponents>;

	class CR_API Component {
	private:
		friend class Entity;
		friend class Scene;
	protected:
		Entity* m_entity;

		virtual void OnInit() {}
		virtual void OnUpdate(float delta) {}
		virtual void OnDraw(const Camera& camera) {}
		virtual void OnDestroy() {}
	public:
		virtual ~Component() {}
	};

	class CR_API Entity {
	private:
		bool m_alive = true;
		std::vector<ref<Component>> m_components;

		ComponentArray m_componentArray;
		ComponentBitset m_componentBitset;

		unsigned long m_id;

		friend class Scene;
	public:
		Scene* m_scene;

		std::string m_name;

		inline unsigned long GetID() const { return m_id; }

		void Update(float delta);
		void Draw(const Camera& camera);

		virtual ~Entity();

		inline void Destroy() { m_alive = false; }

		template <typename T>
		bool HasComponent() const {
			return m_componentBitset[GetComponentTypeID<T>()];
		}

		template <typename T>
		T* GetComponent() const {
			if (!HasComponent<T>()) { return NULL; }
			auto ptr(m_componentArray[GetComponentTypeID<T>()]);
			return static_cast<T*>(ptr);
		}

		template <typename T, typename... TArgs>
		T* AddComponent(TArgs&&... args) {
			if (HasComponent<T>()) {
				return GetComponent<T>();
			}

			ref<Component> c(new T(std::forward<TArgs>(args)...));
			c->m_entity = this;
			m_components.emplace_back(std::move(c));

			m_componentArray[GetComponentTypeID<T>()] = c.get();
			m_componentBitset[GetComponentTypeID<T>()] = true;

			c->OnInit();

			return static_cast<T*>(c.get());
		}

		template <typename T>
		void RemoveComponent() {
			if (!HasComponent<T>()) {
				return;
			}

			m_componentArray[GetComponentTypeID<T>()]->OnDestroy();

			m_components.erase(
				std::remove_if(std::begin(m_components), std::end(m_components), 
					[this](const ref<Component>& component) {
						return m_componentArray[GetComponentTypeID<T>()] == component.get();
					}),
				std::end(m_components)
			);

			m_componentArray[GetComponentTypeID<T>()] = NULL;
			m_componentBitset[GetComponentTypeID<T>()] = false;
		}
	};
}