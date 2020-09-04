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

         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform1f("ambientStrength", sceneManager.GetConfig()->ambientStrength);
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("ambientColor", sceneManager.GetConfig()->ambientColor.x, sceneManager.GetConfig()->ambientColor.y, sceneManager.GetConfig()->ambientColor.z);
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("directionalLightPos", sceneManager.GetConfig()->directionalLightPos.x, sceneManager.GetConfig()->directionalLightPos.y, sceneManager.GetConfig()->directionalLightPos.z);

         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("material.color", ecs.GetComponent<ModelComponent>(ent)->material.color.x, ecs.GetComponent<ModelComponent>(ent)->material.color.y, ecs.GetComponent<ModelComponent>(ent)->material.color.z);
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform1f("material.specularStrength", ecs.GetComponent<ModelComponent>(ent)->material.specularStrength);

         ecs.GetComponent<ModelComponent>(ent)->model.Render();
      }
   }
}
#endif /* end of include guard: MODELCOMP_H */
