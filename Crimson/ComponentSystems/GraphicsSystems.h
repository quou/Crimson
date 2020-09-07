#ifndef MODELCOMP_H
#define MODELCOMP_H

#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Graphics/Model.h"
#include "Graphics/Camera.h"
#include "Graphics/Material.h"
#include "SceneManagement/SceneManager.h"
#include "Graphics/Lighting/PointLight.h"
#include "Transform.h"
#include "SLECS.h"

namespace Crimson {
   struct ModelComponent {
      Texture texture;
      Shader shader;
      Model model;
      Material material;
   };

   static void RenderModels(ECS& ecs, Camera& camera, SceneManager& sceneManager) {
      sceneManager.m_skybox.Render(camera);

      std::vector<std::pair<glm::vec3, PointLight*>> lights;
      for (EntityHandle ent : System<Transform, PointLight>(ecs)) {
         lights.push_back({ecs.GetComponent<Transform>(ent)->worldPosition, ecs.GetComponent<PointLight>(ent)});
      }

      int modelCount = 0;
      for (EntityHandle ent : System<Transform, ModelComponent>(ecs)) {
         glm::mat4 model = GetModelFromTransform(*ecs.GetComponent<Transform>(ent));

         ecs.GetComponent<ModelComponent>(ent)->texture.Bind(modelCount);
         ecs.GetComponent<ModelComponent>(ent)->shader.Bind();

         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform1i("tex", modelCount);
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("cameraPosition", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniformMatrix4("view", camera.GetViewProjection());
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniformMatrix4("modl", model);

         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("directionalLight.direction", sceneManager.GetConfig()->directionalLight.direction.x, sceneManager.GetConfig()->directionalLight.direction.y, sceneManager.GetConfig()->directionalLight.direction.z);
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("directionalLight.ambient", sceneManager.GetConfig()->directionalLight.ambient.x, sceneManager.GetConfig()->directionalLight.ambient.y, sceneManager.GetConfig()->directionalLight.ambient.z);
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("directionalLight.diffuse", sceneManager.GetConfig()->directionalLight.diffuse.x, sceneManager.GetConfig()->directionalLight.diffuse.y, sceneManager.GetConfig()->directionalLight.diffuse.z);
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("directionalLight.specular", sceneManager.GetConfig()->directionalLight.specular.x, sceneManager.GetConfig()->directionalLight.specular.y, sceneManager.GetConfig()->directionalLight.specular.z);


         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform1i("pointLightCount", lights.size());

         int i = 0;
         for (auto& light : lights) {
            ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("pointLights[" + std::to_string(i) + "].position", light.first.x, light.first.y, light.first.z);
            ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform1f("pointLights[" + std::to_string(i) + "].constant", light.second->constant);
            ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform1f("pointLights[" + std::to_string(i) + "].linear", light.second->linear);
            ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform1f("pointLights[" + std::to_string(i) + "].quadratic", light.second->quadratic);
            ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("pointLights[" + std::to_string(i) + "].ambient", light.second->ambient.x, light.second->ambient.y, light.second->ambient.z);
            ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("pointLights[" + std::to_string(i) + "].diffuse", light.second->diffuse.x, light.second->diffuse.y, light.second->diffuse.z);
            ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("pointLights[" + std::to_string(i) + "].specular", light.second->specular.x, light.second->specular.y, light.second->specular.z);
            i++;
         }


         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("material.ambient", ecs.GetComponent<ModelComponent>(ent)->material.ambient.x, ecs.GetComponent<ModelComponent>(ent)->material.ambient.y, ecs.GetComponent<ModelComponent>(ent)->material.ambient.z);
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("material.diffuse", ecs.GetComponent<ModelComponent>(ent)->material.diffuse.x, ecs.GetComponent<ModelComponent>(ent)->material.diffuse.y, ecs.GetComponent<ModelComponent>(ent)->material.diffuse.z);
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform3f("material.specular", ecs.GetComponent<ModelComponent>(ent)->material.specular.x, ecs.GetComponent<ModelComponent>(ent)->material.specular.y, ecs.GetComponent<ModelComponent>(ent)->material.specular.z);
         ecs.GetComponent<ModelComponent>(ent)->shader.SetUniform1f("material.shininess", ecs.GetComponent<ModelComponent>(ent)->material.shininess);

         ecs.GetComponent<ModelComponent>(ent)->model.Render();

         modelCount++;
         if (modelCount > 32) {
            modelCount = 0;
         }
      }
   }
}
#endif /* end of include guard: MODELCOMP_H */
