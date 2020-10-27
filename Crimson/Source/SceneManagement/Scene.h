#pragma once

#include <entt.hpp>

#include "Renderer/Renderer.h"
#include "Renderer/Renderer3D/LightScene.h"

namespace Crimson {

	class Entity;

	class Scene {
	private:
		entt::registry m_registry;

		friend class Entity;
	public:
		Scene();
		~Scene();

		std::shared_ptr<LightScene> m_lightScene;

		Entity CreateEntity();

		void Update(float delta);

		void UpdateViewport(std::pair<int, int> size);
	};

}
