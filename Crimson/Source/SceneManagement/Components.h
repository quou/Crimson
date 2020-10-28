#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/Renderer3D/Mesh.h"
#include "Renderer/Renderer3D/Camera.h"
#include "Renderer/Material.h"

namespace Crimson {
	struct TransformComponent {
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale = glm::vec3(1);

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

		CameraComponent(std::pair<int, int> size, float fov, float near=0.01f, float far=100.0f) : camera(size, fov, near, far) {}
	};
}
