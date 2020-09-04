#ifndef MODELCOMP_H
#define MODELCOMP_H

#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Graphics/Model.h"
#include "Graphics/Camera.h"
#include "Graphics/Material.h"
#include "SceneManagement/SceneManager.h"
#include "SLECS.h"
#include "Transform.h"

namespace Crimson {
   struct ModelComponent {
      Texture texture;
      Shader shader;
      Model model;
      Material material;
   };

   static void RenderModels(ECS& ecs, Camera& camera, SceneManager& sceneManager) {
      for (EntityHandle ent : System<Transform, ModelComponent>(ecs)) {
         glm::mat4 model = GetModelFromTransform(*ecs.GetComponent<Transform>(ent));

         ecs.GetComponent<ModelComponent>(ent)->texture.Bind(0);
         ecs.GetComponent<ModelComponent>(ent)->shader.Bind();

         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform1i("tex", 0);
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("cameraPosition", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniformMatrix4("view", camera.GetViewProjection());
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniformMatrix4("modl", model);

         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("light.direction", sceneManager.GetConfig()->directionalLightDir.x, sceneManager.GetConfig()->directionalLightDir.y, sceneManager.GetConfig()->directionalLightDir.z);
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("light.ambient",  0.2f, 0.2f, 0.2f);
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("light.diffuse",  0.5f, 0.5f, 0.5f);
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("light.specular", 1.0f, 1.0f, 1.0f);

         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("material.ambient", ecs.GetComponent<ModelComponent>(ent)->material.ambient.x, ecs.GetComponent<ModelComponent>(ent)->material.ambient.y, ecs.GetComponent<ModelComponent>(ent)->material.ambient.z);
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("material.diffuse", ecs.GetComponent<ModelComponent>(ent)->material.diffuse.x, ecs.GetComponent<ModelComponent>(ent)->material.diffuse.y, ecs.GetComponent<ModelComponent>(ent)->material.diffuse.z);
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("material.specular", ecs.GetComponent<ModelComponent>(ent)->material.specular.x, ecs.GetComponent<ModelComponent>(ent)->material.specular.y, ecs.GetComponent<ModelComponent>(ent)->material.specular.z);
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform1f("material.shininess", ecs.GetComponent<ModelComponent>(ent)->material.shininess);

         ecs.GetComponent<ModelComponent>(ent)->model.Render();
      }
   }
}
#endif /* end of include guard: MODELCOMP_H */
