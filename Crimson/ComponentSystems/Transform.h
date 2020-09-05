#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <algorithm>

#include "SLECS.h"

namespace Crimson {
   struct Transform {
      std::string name;
      glm::vec3 position;
      glm::vec3 rotation;
      glm::vec3 scale;

      glm::vec3 worldPosition;
      glm::vec3 worldRotation;
      glm::vec3 worldScale;

      EntityHandle parent = 0;
      std::vector<EntityHandle> children;
   };

   static void AddParent(Transform* transform, EntityHandle entity) {
      transform->parent = entity;
   }
   static void Unparent(Transform* transform) {
      transform->parent = 0;
   }

   static void AddChild(Transform* transform, EntityHandle entity) {
      transform->children.push_back(entity);
   }
   static void RemoveChild(Transform* transform, EntityHandle entity) {
      transform->children.erase(std::remove(transform->children.begin(), transform->children.end(), entity), transform->children.end());
   }

   static glm::mat4 GetModelFromTransform(const Transform& transform) {
      glm::vec3 pos = transform.worldPosition;
      glm::vec3 rot = transform.worldRotation;
      glm::vec3 scale = transform.worldScale;

      glm::mat4 posMatrix = glm::translate(glm::mat4(1.0f), pos);

      glm::mat4 rotxMatrix = glm::rotate(glm::mat4(1.0f), rot.x, glm::vec3(1,0,0));
      glm::mat4 rotyMatrix = glm::rotate(glm::mat4(1.0f), rot.y, glm::vec3(0,1,0));
      glm::mat4 rotzMatrix = glm::rotate(glm::mat4(1.0f), rot.z, glm::vec3(0,0,1));

      glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

      glm::mat4 rotMatrix = rotzMatrix * rotyMatrix * rotxMatrix;

      return posMatrix * rotMatrix * scaleMatrix;
   }

   static glm::mat4 GetLocalModelFromTransform(const Transform& transform) {
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

   static void UpdateTransforms(ECS& ecs) {
      for (EntityHandle ent : System<Transform>(ecs)) {
         Transform* t = ecs.GetComponent<Transform>(ent);

         if (t->parent) {
            t->worldPosition = ecs.GetComponent<Transform>(t->parent)->worldPosition + t->position;
            t->worldScale = ecs.GetComponent<Transform>(t->parent)->worldScale * t->scale;
            t->worldRotation = ecs.GetComponent<Transform>(t->parent)->worldRotation + t->rotation;
         } else {
            t->worldPosition = t->position;
            t->worldScale = t->scale;
            t->worldRotation = t->rotation;
         }
      }
   }
}
#endif /* end of include guard: TRANSFORM_H */
