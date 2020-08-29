#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

namespace Crimson {
   struct Transform {
      std::string name;
      glm::vec3 position;
      glm::vec3 rotation;
      glm::vec3 scale;
   };

   static glm::mat4 GetModelFromTransform(const Transform& transform) {
      glm::vec3 pos = transform.position;
      glm::vec3 rot = transform.rotation;
      glm::vec3 scale = transform.scale;

      glm::mat4 posMatrix = glm::translate(glm::mat4(1.0f), pos);

      glm::mat4 rotxMatrix = glm::rotate(glm::mat4(1.0f), rot.x, glm::vec3(1,0,0));
      glm::mat4 rotyMatrix = glm::rotate(glm::mat4(1.0f), rot.y, glm::vec3(0,1,0));
      glm::mat4 rotzMatrix = glm::rotate(glm::mat4(1.0f), rot.z, glm::vec3(0,0,1));

      glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

      glm::mat4 rotMatrix = rotzMatrix * rotyMatrix * rotxMatrix;

      return posMatrix * rotMatrix * scaleMatrix;
   }
}
#endif /* end of include guard: TRANSFORM_H */
