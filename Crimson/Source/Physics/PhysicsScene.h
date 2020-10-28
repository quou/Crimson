#pragma once

#include <reactphysics3d/reactphysics3d.h>

namespace rp3d = ::reactphysics3d;

namespace Crimson {
	class PhysicsScene {
	private:
		rp3d::PhysicsCommon m_common;
		rp3d::PhysicsWorld* m_world;

		float m_accumulator{0.0f};

		friend class Rigidbody;
	public:
		PhysicsScene();
		~PhysicsScene();

		void Update(float delta);
	};
}
