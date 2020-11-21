#include "Rigidbody.h"

#include "PhysicsScene.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace Crimson {
	Rigidbody::Rigidbody(PhysicsScene* scene, const glm::vec3& position, const glm::vec3& rotation) : m_scene(scene) {
		rp3d::Vector3 p(position.x, position.y, position.z);

		glm::quat q = glm::toQuat(glm::orientate3(glm::vec3(
			glm::radians(rotation.y),
			glm::radians(rotation.z),
			glm::radians(rotation.x))));

		rp3d::Quaternion o(q.x, q.y, q.z, q.w);

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
		glm::quat q(
			m_body->getTransform().getOrientation().y,
			m_body->getTransform().getOrientation().z,
			m_body->getTransform().getOrientation().x,
			m_body->getTransform().getOrientation().w);

		glm::vec3 result = glm::degrees(glm::eulerAngles(q));

		return glm::vec3(result.x, result.y, result.z);
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

	void Rigidbody::ApplyForce(glm::vec3 force) {
		m_body->applyForceToCenterOfMass(rp3d::Vector3(force.x, force.y, force.z));
	}

	void Rigidbody::SetCOG(glm::vec3 cog) {
		m_body->setLocalCenterOfMass(rp3d::Vector3(cog.x, cog.y, cog.z));
	}

	void Rigidbody::ApplyForceAtPosition(glm::vec3 position, glm::vec3 force) {
		m_body->applyForceAtLocalPosition(rp3d::Vector3(force.x, force.y, force.z), rp3d::Vector3(position.x, position.y, position.z));
	}

	void Rigidbody::ApplyTorque(glm::vec3 torque) {
		m_body->applyTorque(rp3d::Vector3(torque.x, torque.y, torque.z));
	}

	void Rigidbody::EnableGravity(bool enable) {
		m_body->enableGravity(enable);
	}

	float Rigidbody::GetBounciness() {
		for (int i = 0; i < m_body->getNbColliders(); i++) {
			auto& mat = m_body->getCollider(i)->getMaterial();
			return mat.getBounciness();
		}
		return 0.0f;
	}

	float Rigidbody::GetFriction() {
		for (int i = 0; i < m_body->getNbColliders(); i++) {
			auto& mat = m_body->getCollider(i)->getMaterial();
			return mat.getFrictionCoefficient();
		}
		return 0.0f;
	}

	void Rigidbody::SetBounciness(float bounciness) {
		for (int i = 0; i < m_body->getNbColliders(); i++) {
			auto& mat = m_body->getCollider(i)->getMaterial();
			mat.setBounciness(bounciness);
			m_body->getCollider(i)->setMaterial(mat);
		}
	}

	void Rigidbody::SetFriction(float friction) {
		for (int i = 0; i < m_body->getNbColliders(); i++) {
			auto& mat = m_body->getCollider(i)->getMaterial();
			mat.setFrictionCoefficient(friction);
			m_body->getCollider(i)->setMaterial(mat);
		}
	}

	void Rigidbody::SetTransform(glm::vec3 position, glm::vec3 rotation) {
		rp3d::Vector3 p(position.x, position.y, position.z);
		rp3d::Quaternion o = rp3d::Quaternion::identity();
		o.x = rotation.x; o.y = rotation.y; o.z = rotation.z;

		rp3d::Transform transform(p, o);

		m_body->setTransform(transform);
	}

	float Rigidbody::GetMass() const {
		return m_body->getMass();
	}

	void Rigidbody::SetMass(float mass) {
		m_body->setMass(mass);
	}
}
