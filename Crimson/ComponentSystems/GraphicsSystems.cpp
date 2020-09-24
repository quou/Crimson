#include "GraphicsSystems.h"


namespace Crimson {
   std::vector<std::pair<glm::vec3, PointLight>> lights;

   void LightingSystem::Update(ECS& ecs) {
      lights.clear();
      for (const auto& ent : m_entities) {
         lights.push_back({ecs.GetComponent<Transform>(ent).worldPosition, ecs.GetComponent<PointLight>(ent)});
      }
   }

   void GraphicsSystem::RenderPass(ECS& ecs, Camera& camera, SceneManager& sceneManager) {
      for (const auto& ent : m_entities) {
         glm::mat4 model = GetModelFromTransform(ecs.GetComponent<Transform>(ent));

         ecs.GetComponent<ModelComponent>(ent).shader.Bind();

         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, sceneManager.GetShadowmap()->GetOutput());
         ecs.GetComponent<ModelComponent>(ent).shader.SetUniform1i("shadowMap", 0);

         ecs.GetComponent<ModelComponent>(ent).texture.Bind(1);
         ecs.GetComponent<ModelComponent>(ent).shader.SetUniform1i("tex", 1);

         ecs.GetComponent<ModelComponent>(ent).shader.SetUniform3f("cameraPosition", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
         ecs.GetComponent<ModelComponent>(ent).shader.SetUniformMatrix4("view", camera.GetViewProjection());
         ecs.GetComponent<ModelComponent>(ent).shader.SetUniformMatrix4("modl", model);

         float near_plane = 0.0f, far_plane = 100.0f;
         glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
         glm::mat4 lightView = glm::lookAt(-sceneManager.GetConfig()->directionalLight.direction * 20.0f,
                                           glm::vec3( 0.0f, 0.0f,  0.0f),
                                           glm::vec3( 0.0f, 1.0f,  0.0f));

         glm::mat4 lightSpaceMatrix = lightProjection * lightView;
         ecs.GetComponent<ModelComponent>(ent).shader.SetUniformMatrix4("directionalLightTransform", lightSpaceMatrix);

         ecs.GetComponent<ModelComponent>(ent).shader.SetUniform3f("directionalLight.direction", sceneManager.GetConfig()->directionalLight.direction.x, sceneManager.GetConfig()->directionalLight.direction.y, sceneManager.GetConfig()->directionalLight.direction.z);
         ecs.GetComponent<ModelComponent>(ent).shader.SetUniform3f("directionalLight.ambient", sceneManager.GetConfig()->directionalLight.ambient.x, sceneManager.GetConfig()->directionalLight.ambient.y, sceneManager.GetConfig()->directionalLight.ambient.z);
         ecs.GetComponent<ModelComponent>(ent).shader.SetUniform3f("directionalLight.diffuse", sceneManager.GetConfig()->directionalLight.diffuse.x, sceneManager.GetConfig()->directionalLight.diffuse.y, sceneManager.GetConfig()->directionalLight.diffuse.z);
         ecs.GetComponent<ModelComponent>(ent).shader.SetUniform3f("directionalLight.specular", sceneManager.GetConfig()->directionalLight.specular.x, sceneManager.GetConfig()->directionalLight.specular.y, sceneManager.GetConfig()->directionalLight.specular.z);


         ecs.GetComponent<ModelComponent>(ent).shader.SetUniform1i("pointLightCount", lights.size());

         int i = 0;
         for (auto& light : lights) {
            ecs.GetComponent<ModelComponent>(ent).shader.SetUniform3f("pointLights[" + std::to_string(i) + "].position", light.first.x, light.first.y, light.first.z);
            ecs.GetComponent<ModelComponent>(ent).shader.SetUniform1f("pointLights[" + std::to_string(i) + "].constant", light.second.constant);
            ecs.GetComponent<ModelComponent>(ent).shader.SetUniform1f("pointLights[" + std::to_string(i) + "].linear", light.second.linear);
            ecs.GetComponent<ModelComponent>(ent).shader.SetUniform1f("pointLights[" + std::to_string(i) + "].quadratic", light.second.quadratic);
            ecs.GetComponent<ModelComponent>(ent).shader.SetUniform3f("pointLights[" + std::to_string(i) + "].ambient", light.second.ambient.x, light.second.ambient.y, light.second.ambient.z);
            ecs.GetComponent<ModelComponent>(ent).shader.SetUniform3f("pointLights[" + std::to_string(i) + "].diffuse", light.second.diffuse.x, light.second.diffuse.y, light.second.diffuse.z);
            ecs.GetComponent<ModelComponent>(ent).shader.SetUniform3f("pointLights[" + std::to_string(i) + "].specular", light.second.specular.x, light.second.specular.y, light.second.specular.z);
            i++;
         }


         ecs.GetComponent<ModelComponent>(ent).shader.SetUniform3f("material.ambient", ecs.GetComponent<ModelComponent>(ent).material.ambient.x, ecs.GetComponent<ModelComponent>(ent).material.ambient.y, ecs.GetComponent<ModelComponent>(ent).material.ambient.z);
         ecs.GetComponent<ModelComponent>(ent).shader.SetUniform3f("material.diffuse", ecs.GetComponent<ModelComponent>(ent).material.diffuse.x, ecs.GetComponent<ModelComponent>(ent).material.diffuse.y, ecs.GetComponent<ModelComponent>(ent).material.diffuse.z);
         ecs.GetComponent<ModelComponent>(ent).shader.SetUniform3f("material.specular", ecs.GetComponent<ModelComponent>(ent).material.specular.x, ecs.GetComponent<ModelComponent>(ent).material.specular.y, ecs.GetComponent<ModelComponent>(ent).material.specular.z);
         ecs.GetComponent<ModelComponent>(ent).shader.SetUniform1f("material.shininess", ecs.GetComponent<ModelComponent>(ent).material.shininess);

         ecs.GetComponent<ModelComponent>(ent).model.Render();
      }
   }

   void GraphicsSystem::ShadowPass(ECS& ecs, Camera& camera, SceneManager& sceneManager) {
      for (const auto& ent : m_entities) {
         glm::mat4 model = GetModelFromTransform(ecs.GetComponent<Transform>(ent));

         sceneManager.GetShadowmap()->GetShader()->Bind();

         float near_plane = 0.0f, far_plane = 100.0f;
         glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
         glm::mat4 lightView = glm::lookAt(-sceneManager.GetConfig()->directionalLight.direction * 20.0f,
                                           glm::vec3( 0.0f, 0.0f,  0.0f),
                                           glm::vec3( 0.0f, 1.0f,  0.0f));

         glm::mat4 lightSpaceMatrix = lightProjection * lightView;

         sceneManager.GetShadowmap()->GetShader()->SetUniformMatrix4("modl", model);
         sceneManager.GetShadowmap()->GetShader()->SetUniformMatrix4("view", camera.GetViewProjection());
         sceneManager.GetShadowmap()->GetShader()->SetUniformMatrix4("lightSpaceMatrix", lightSpaceMatrix);

         ecs.GetComponent<ModelComponent>(ent).model.Render();
      }
   }

   void GraphicsSystem::RenderShadows(ECS& ecs, SceneManager& sceneManager) {
      if (sceneManager.GetCurrentCamera()) {
         sceneManager.GetShadowmap()->BeginRender();
         ShadowPass(ecs, *sceneManager.GetCurrentCamera(), sceneManager);
         sceneManager.GetShadowmap()->EndRender();
      }
   }

   void GraphicsSystem::Render(ECS& ecs, SceneManager& sceneManager) {
      if (sceneManager.GetCurrentCamera()) {
         sceneManager.m_skybox.Render(*sceneManager.GetCurrentCamera());
         RenderPass(ecs, *sceneManager.GetCurrentCamera(), sceneManager);
      }
   }
}
