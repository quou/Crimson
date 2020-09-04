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

         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("light.direction", sceneManager.GetConfig()->directionalLight.direction.x, sceneManager.GetConfig()->directionalLight.direction.y, sceneManager.GetConfig()->directionalLight.direction.z);
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("light.ambient", sceneManager.GetConfig()->directionalLight.ambient.x, sceneManager.GetConfig()->directionalLight.ambient.y, sceneManager.GetConfig()->directionalLight.ambient.z);
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("light.diffuse", sceneManager.GetConfig()->directionalLight.diffuse.x, sceneManager.GetConfig()->directionalLight.diffuse.y, sceneManager.GetConfig()->directionalLight.diffuse.z);
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("light.specular", sceneManager.GetConfig()->directionalLight.specular.x, sceneManager.GetConfig()->directionalLight.specular.y, sceneManager.GetConfig()->directionalLight.specular.z);


         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("material.ambient", ecs.GetComponent<ModelComponent>(ent)->material.ambient.x, ecs.GetComponent<ModelComponent>(ent)->material.ambient.y, ecs.GetComponent<ModelComponent>(ent)->material.ambient.z);
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("material.diffuse", ecs.GetComponent<ModelComponent>(ent)->material.diffuse.x, ecs.GetComponent<ModelComponent>(ent)->material.diffuse.y, ecs.GetComponent<ModelComponent>(ent)->material.diffuse.z);
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("material.specular", ecs.GetComponent<ModelComponent>(ent)->material.specular.x, ecs.GetComponent<ModelComponent>(ent)->material.specular.y, ecs.GetComponent<ModelComponent>(ent)->material.specular.z);
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform1f("material.shininess", ecs.GetComponent<ModelComponent>(ent)->material.shininess);

         ecs.GetComponent<ModelComponent>(ent)->model.Render();
      }
   }
}
#endif /* end of include guard: MODELCOMP_H */
