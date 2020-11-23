#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <reactphysics3d/reactphysics3d.h>

namespace rp3d = ::reactphysics3d;

namespace Crimson {
	class PhysicsScene;

	class Rigidbody {
	private:
		PhysicsScene* m_scene;

		std::vector<rp3d::Collider*> m_colliders;
		bool m_isKinematic;
		rp3d::RigidBody* m_body;

		friend class Scene;
	public:
		Rigidbody() {}
		Rigidbody(PhysicsScene* scene, const glm::vec3& position = glm::vec3(0.0f), const glm::quat& rotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		~Rigidbody();

		void AddBoxCollider(const glm::vec3& halfExtents);
		void AddSphereCollider(float radius);

		void SetCOG(glm::vec3 cog);
		void ApplyForce(glm::vec3 force);
		void ApplyForceAtPosition(glm::vec3 position, glm::vec3 force);
		void ApplyTorque(glm::vec3 torque);
		void EnableGravity(bool enable);
		float GetMass() const;
		void SetMass(float mass);

		void SetBounciness(float bounciness);
		void SetFriction(float friction);
		float GetBounciness();
		float GetFriction();

		glm::vec3 GetPosition();
		glm::quat GetRotation();

		inline glm::vec3 GetCOG() {return glm::vec3(m_body->getLocalCenterOfMass().x, m_body->getLocalCenterOfMass().y, m_body->getLocalCenterOfMass().z);}

		inline bool GetKinematic() const {return m_isKinematic;}
		void SetKinematic(bool set);


		void SetTransform(glm::vec3 position, glm::vec3 rotation);
	};
}
