#include "PhysicsScene.h"

#include "SceneManagement/Scene.h"

namespace Crimson {
	PhysicsScene::PhysicsScene(Scene* scene) : m_scene(scene), m_eventListener(this) {
		m_world = m_common.createPhysicsWorld();
		m_world->setEventListener(&m_eventListener);
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

	void PhysicsScene::Contact(rp3d::CollisionBody* body, rp3d::CollisionBody* other) {
		m_scene->Contact(body, other);
	}
}
