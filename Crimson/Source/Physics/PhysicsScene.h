#pragma once

#include "EventListener.h"

#include <reactphysics3d/reactphysics3d.h>

namespace rp3d = ::reactphysics3d;

namespace Crimson {
	class Scene;

	class PhysicsScene {
	private:
		rp3d::PhysicsCommon m_common;
		rp3d::PhysicsWorld* m_world;

		float m_accumulator{0.0f};

		friend class Rigidbody;

		EventListener m_eventListener;

		Scene* m_scene;
	public:
		PhysicsScene(Scene* scene);
		~PhysicsScene();

		void Update(float delta);

		void ContactStay(rp3d::CollisionBody* body, rp3d::CollisionBody* other);
		void ContactEnter(rp3d::CollisionBody* body, rp3d::CollisionBody* other);
		void ContactExit(rp3d::CollisionBody* body, rp3d::CollisionBody* other);
	};
}
