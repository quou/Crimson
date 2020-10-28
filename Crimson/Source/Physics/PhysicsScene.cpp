#include "PhysicsScene.h"

namespace Crimson {
	PhysicsScene::PhysicsScene() {
		m_world = m_common.createPhysicsWorld();
	}

	PhysicsScene::~PhysicsScene() {

	}

	void PhysicsScene::Update(float delta) {
		const float timeStep = 1.0f / 60.0f;

		m_accumulator += delta;

		while (m_accumulator >= timeStep) {
			m_world->update(timeStep);

			m_accumulator -= delta;
		}
	}
}
