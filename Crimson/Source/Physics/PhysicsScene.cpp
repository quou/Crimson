#include "PhysicsScene.h"

#include "SceneManagement/Scene.h"

#include <GLFW/glfw3.h>

namespace Crimson {
	PhysicsScene::PhysicsScene(Scene* scene) : m_scene(scene), m_eventListener(this) {
		m_world = m_common.createPhysicsWorld();
		m_world->setEventListener(&m_eventListener);
		m_world->enableSleeping(false);
	}

	PhysicsScene::~PhysicsScene() {

	}

	void PhysicsScene::Update(float delta) {
		float timeSinceStart = (float)glfwGetTime();

		const float timeStep = 1.0f / 60.0f;

		m_accumulator += delta;

		while (m_accumulator >= timeStep) {
			m_world->update(timeStep);

			m_scene->PhysicsUpdate(timeStep);

			m_accumulator -= delta * m_updateTime;
		}

		m_updateTime = timeSinceStart - m_oldTimeSinceStart;
		m_oldTimeSinceStart = timeSinceStart;
	}

	void PhysicsScene::ContactStay(rp3d::CollisionBody* body, rp3d::CollisionBody* other) {
		m_scene->ContactStay(body, other);
	}

	void PhysicsScene::ContactEnter(rp3d::CollisionBody* body, rp3d::CollisionBody* other) {
		m_scene->ContactEnter(body, other);
	}

	void PhysicsScene::ContactExit(rp3d::CollisionBody* body, rp3d::CollisionBody* other) {
		m_scene->ContactExit(body, other);
	}
}
