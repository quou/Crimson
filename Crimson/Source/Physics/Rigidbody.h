#pragma once

#include <glm/glm.hpp>

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
		Rigidbody(PhysicsScene* scene, const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& rotation = glm::vec3(0.0f));
		~Rigidbody();

		void AddBoxCollider(const glm::vec3& halfExtents);
		void AddSphereCollider(float radius);

		void SetCOG(glm::vec3 cog);
		void ApplyForce(glm::vec3 force);
		void ApplyForceAtPosition(glm::vec3 position, glm::vec3 force);
		void ApplyTorque(glm::vec3 torque);

		glm::vec3 GetPosition();
		glm::vec3 GetRotation();

		inline bool GetKinematic() const {return m_isKinematic;}
		void SetKinematic(bool set);

		float GetMass() const;
		void SetMass(float mass);

		void SetTransform(glm::vec3 position, glm::vec3 rotation);
	};
}
