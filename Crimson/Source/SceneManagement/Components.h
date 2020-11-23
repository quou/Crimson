#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/Renderer3D/Mesh.h"
#include "Renderer/Renderer3D/Camera.h"
#include "Renderer/Material.h"
#include "Physics/Rigidbody.h"

namespace Crimson {
	struct TransformComponent {
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale = glm::vec3(1);

		std::string name;
		std::string tag;
		std::string guid;

		TransformComponent() = default;
		glm::mat4 GetTransform() {
			glm::mat4 positionMatrix = glm::translate(glm::mat4(1.0f), position);
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
			glm::mat4 rotationMatrix = glm::toMat4(rotation);

			return positionMatrix * rotationMatrix * scaleMatrix;
		}
	};

	struct MeshFilterComponent {
		std::string path;

		MeshFilterComponent() = default;
		MeshFilterComponent(const std::string& p) : path(p) {}
	};

	struct MaterialComponent {
		std::string path;

		MaterialComponent() = default;
		MaterialComponent(const std::string& p) : path(p) {}
	};

	struct CameraComponent {
		Camera camera;
		bool active;

		CameraComponent() = default;
		CameraComponent(std::pair<int, int> size, float fov, float near=0.01f, float far=100.0f, bool active=true) : camera(size, fov, near, far), active(active) {}
	};

	struct BoxColliderComponent {
		glm::vec3 extents;

		BoxColliderComponent() = default;
		BoxColliderComponent(const glm::vec3& extents) : extents(extents) {}
	};

	struct SphereColliderComponent {
		float radius;

		SphereColliderComponent() = default;
		SphereColliderComponent(float radius) : radius(radius) {}
	};

	struct PhysicsComponent {
		bool useGravity;
		float mass;
		float friction;
		float bounciness;
		bool isKinematic;
		glm::vec3 cog;

		Rigidbody* context;

		void ApplyForce(const glm::vec3& force) {
			context->ApplyForce(force);
		}

		void ApplyForceAtPosition(const glm::vec3& position, const glm::vec3& force) {
			context->ApplyForceAtPosition(position, force);
		}

		void ApplyTorque(const glm::vec3& torque) {
			context->ApplyTorque(torque);
		}

		PhysicsComponent() = default;

		PhysicsComponent(bool useGravity, float mass, float friction, float bounciness, bool isKinematic, glm::vec3 cog) :
			useGravity(useGravity), mass(mass), friction(friction), bounciness(bounciness), isKinematic(isKinematic), cog(cog) {}
	};

	struct ScriptComponent {
		std::string className;

		unsigned int id;

		ScriptComponent() = default;
		ScriptComponent(const std::string& className) : className(className) {}
	};

	struct AmbientLightComponent {
		glm::vec3 color;
		float intensity;

		AmbientLightComponent() = default;
		AmbientLightComponent(glm::vec3 color, float intensity) :
		color(color), intensity(intensity) {}
	};

	struct DirectionalLightComponent {
		glm::vec3 color;
		float intensity;

		DirectionalLightComponent() = default;
		DirectionalLightComponent(glm::vec3 color, float intensity) :
		color(color), intensity(intensity) {}
	};

	struct PointLightComponent {
		float constant;
		float linear;
		float quadratic;

		glm::vec3 color;
		float intensity;

		PointLightComponent() = default;

		PointLightComponent(float constant, float linear, float quadratic, glm::vec3 color, float intensity) :
		constant(constant), linear(linear), quadratic(quadratic), color(color), intensity(intensity) {}
	};
}
