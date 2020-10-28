#include "Rigidbody.h"

#include "PhysicsScene.h"

namespace Crimson {
	Rigidbody::Rigidbody(PhysicsScene* scene, const glm::vec3& position, const glm::vec3& rotation) : m_scene(scene) {
		rp3d::Vector3 p(position.x, position.y, position.z);
		rp3d::Quaternion o = rp3d::Quaternion::identity();
		o.x = rotation.x; o.y = rotation.y; o.z = rotation.z;

		rp3d::Transform transform(p, o);

		m_body = m_scene->m_world->createRigidBody(transform);

		m_body->setType(rp3d::BodyType::DYNAMIC);
		m_isKinematic = false;
	}

	Rigidbody::~Rigidbody() {
		m_scene->m_world->destroyRigidBody(m_body);
	}

	glm::vec3 Rigidbody::GetPosition() {
		return glm::vec3(
			m_body->getTransform().getPosition().x,
			m_body->getTransform().getPosition().y,
			m_body->getTransform().getPosition().z);
	}

	glm::vec3 Rigidbody::GetRotation() {
		return glm::vec3(
			m_body->getTransform().getOrientation().x,
			m_body->getTransform().getOrientation().y,
			m_body->getTransform().getOrientation().z);
	}

	void Rigidbody::AddBoxCollider(const glm::vec3& halfExtents) {
		const rp3d::Vector3 e(halfExtents.x, halfExtents.y, halfExtents.z);

		rp3d::BoxShape* shape = m_scene->m_common.createBoxShape(e);

		rp3d::Transform transform = rp3d::Transform::identity();
		rp3d::Collider* col = m_body->addCollider(shape, transform);
		m_colliders.push_back(col);
	}

	void Rigidbody::AddSphereCollider(float radius) {
		rp3d::SphereShape* shape = m_scene->m_common.createSphereShape(radius);

		rp3d::Transform transform = rp3d::Transform::identity();
		rp3d::Collider* col = m_body->addCollider(shape, transform);
		m_colliders.push_back(col);
	}

	void Rigidbody::SetKinematic(bool set) {
		if (set) {
			m_body->setType(rp3d::BodyType::KINEMATIC);
		} else {
			m_body->setType(rp3d::BodyType::DYNAMIC);
		}

		m_isKinematic = set;
	}


	float Rigidbody::GetMass() const {
		return m_body->getMass();
	}

	void Rigidbody::SetMass(float mass) {
		m_body->setMass(mass);
	}
}
