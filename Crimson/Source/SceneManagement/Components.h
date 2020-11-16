#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/Renderer3D/Mesh.h"
#include "Renderer/Renderer3D/Camera.h"
#include "Renderer/Material.h"
#include "Physics/Rigidbody.h"

namespace Crimson {
	struct TransformComponent {
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale = glm::vec3(1);

		std::string name;
		std::string tag;
		std::string guid;

		glm::mat4 GetTransform() {
			glm::mat4 rotxMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1,0,0));
			glm::mat4 rotyMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0,1,0));
			glm::mat4 rotzMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0,0,1));

			glm::mat4 positionMatrix = glm::translate(glm::mat4(1.0f), position);
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
			glm::mat4 rotationMatrix = rotzMatrix * rotyMatrix * rotxMatrix;

			return positionMatrix * rotationMatrix * scaleMatrix;
		}
	};

	struct MeshFilterComponent {
		std::string path;

		MeshFilterComponent(const std::string& p) : path(p) {}
	};

	struct MaterialComponent {
		std::string path;

		MaterialComponent(const std::string& p) : path(p) {}
	};

	struct CameraComponent {
		Camera camera;
		bool active;

		CameraComponent(std::pair<int, int> size, float fov, float near=0.01f, float far=100.0f, bool active=true) : camera(size, fov, near, far), active(active) {}
	};

	struct BoxColliderComponent {
		glm::vec3 extents;

		BoxColliderComponent(const glm::vec3& extents) : extents(extents) {}
	};

	struct SphereColliderComponent {
		float radius;

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

		PhysicsComponent(bool useGravity, float mass, float friction, float bounciness, bool isKinematic, glm::vec3 cog) :
			useGravity(useGravity), mass(mass), friction(friction), bounciness(bounciness), isKinematic(isKinematic), cog(cog) {}
	};

	struct ScriptComponent {
		std::string className;

		unsigned int id;

		ScriptComponent(const std::string& className) : className(className) {}
	};

	struct AmbientLightComponent {
		glm::vec3 color;
		float intensity;

		AmbientLightComponent(glm::vec3 color, float intensity) :
		color(color), intensity(intensity) {}
	};

	struct DirectionalLightComponent {
		glm::vec3 color;
		float intensity;

		DirectionalLightComponent(glm::vec3 color, float intensity) :
		color(color), intensity(intensity) {}
	};

	struct PointLightComponent {
		float constant;
		float linear;
		float quadratic;

		glm::vec3 color;
		float intensity;

		PointLightComponent(float constant, float linear, float quadratic, glm::vec3 color, float intensity) :
		constant(constant), linear(linear), quadratic(quadratic), color(color), intensity(intensity) {}
	};
}
