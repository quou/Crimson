#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Crimson {
	struct Transform {
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;

		inline glm::mat4 GetModel() const {
			glm::mat4 rotxMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1,0,0));
			glm::mat4 rotyMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0,1,0));
			glm::mat4 rotzMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0,0,1));

			glm::mat4 positionMatrix = glm::translate(glm::mat4(1.0f), position);
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
			glm::mat4 rotationMatrix = rotzMatrix * rotyMatrix * rotxMatrix;

			return positionMatrix * rotationMatrix * scaleMatrix;
		}
	};
}
