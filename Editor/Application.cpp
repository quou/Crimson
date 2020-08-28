#include <Crimson.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GUI.h"

class App : public Crimson::Application {
private:
   Crimson::DirectionalLight m_directional;
   Crimson::Camera m_camera;

   Crimson::Material m_shinyMaterial;

   glm::mat4 m_modl;
   glm::mat4 m_view;

   float m_curAngle = 0.0f;

   EntityHandle m_monkey;

   GUI m_gui;
public:
   App() :
      m_camera(glm::vec3(0,0,-5), 45.0f, 1366/768, 0.1f, 100.0f),
      //m_model("Resources/monkey3.obj"),
      //m_texture("Resources/Wood.jpg"),
      //m_shader("Resources/Basic.vert", "Resources/Basic.frag"),
      m_shinyMaterial(1.0f, 32),
      m_directional(glm::vec3(1,1,1), 0.2f, 0.9f, glm::vec3(1,1,0)) {}

   void OnBegin() override  {
      m_gui.Init(GetSDLWindow(), GetSDLGLContext());

      m_monkey = m_ecs.CreateEntity();
      m_ecs.AddComponent<Crimson::Transform>(m_monkey, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
      m_ecs.AddComponent<Crimson::ModelComponent>(m_monkey)->shader.Init("Resources/Basic.vert", "Resources/Basic.frag");
      m_ecs.GetComponent<Crimson::ModelComponent>(m_monkey)->texture.Load("Resources/Wood.jpg");
      m_ecs.GetComponent<Crimson::ModelComponent>(m_monkey)->model.Load("Resources/monkey3.obj");

      m_directional.UseLight(m_ecs.GetComponent<Crimson::ModelComponent>(m_monkey)->shader.GetUniformLocation("directionalLight.ambientIntensity"),
                             m_ecs.GetComponent<Crimson::ModelComponent>(m_monkey)->shader.GetUniformLocation("directionalLight.color"),
                             m_ecs.GetComponent<Crimson::ModelComponent>(m_monkey)->shader.GetUniformLocation("directionalLight.diffuseIntensity"),
                             m_ecs.GetComponent<Crimson::ModelComponent>(m_monkey)->shader.GetUniformLocation("directionalLight.direction"));

      //m_shader.SetUniform3f("eyePosition", m_camera.GetPosition().x, m_camera.GetPosition().y, m_camera.GetPosition().z);
      m_shinyMaterial.UseMaterial(m_ecs.GetComponent<Crimson::ModelComponent>(m_monkey)->shader.GetUniformLocation("material.specularIntensity"),
                                  m_ecs.GetComponent<Crimson::ModelComponent>(m_monkey)->shader.GetUniformLocation("material.shininess"));
   }

   void OnUpdate(float delta) override {
      m_gui.Update(GetEvent());

      m_camera.UpdatePerspective(45.0f, (float)GetDisplay()->GetSize().first/(float)GetDisplay()->GetSize().second, 0.1f, 100.0f);
      //m_shader.SetUniformMatrix4("view", m_camera.GetViewProjection());
      //m_shader.SetUniform3f("eyePosition", m_camera.GetPosition().x, m_camera.GetPosition().y, m_camera.GetPosition().z);

      float pitch = m_camera.GetPitch();
      float yaw = m_camera.GetYaw();

      float rotSpeed = 800.0f;
      float moveSpeed = 10.0f;

      float yoffset = 0.0f;
      float xoffset = 0.0f;

      if (m_keyboard.IsKeyHeld(SDL_SCANCODE_UP)) {
         yoffset = rotSpeed * delta;
      } else if (m_keyboard.IsKeyHeld(SDL_SCANCODE_DOWN)) {
         yoffset = -rotSpeed * delta;
      }

      if (m_keyboard.IsKeyHeld(SDL_SCANCODE_LEFT)) {
         xoffset = -rotSpeed * delta;
      } else if (m_keyboard.IsKeyHeld(SDL_SCANCODE_RIGHT)) {
         xoffset = rotSpeed * delta;
      }

      float sensitivity = 0.1f;
      xoffset *= sensitivity;
      yoffset *= sensitivity;

      yaw += xoffset;
      pitch += yoffset;

      if (pitch > 89.0f) {
         pitch = 89.0f;
      }

      if (pitch < -89.0f) {
         pitch = -89.0f;
      }

      m_camera.SetRotation(yaw, pitch);


      glm::vec3 pos = m_camera.GetPosition();
      float rot = glm::radians(m_camera.GetYaw());
      if (m_keyboard.IsKeyHeld(SDL_SCANCODE_W)) {
         pos.x -= (float)cos(rot) * moveSpeed * -1.0f * delta;
         pos.z += (float)sin(rot) * moveSpeed * delta;
      } else if (m_keyboard.IsKeyHeld(SDL_SCANCODE_S)) {
         pos.x -= (float)cos(rot) * moveSpeed * 1.0f * delta;
         pos.z -= (float)sin(rot) * moveSpeed * delta;
      }
      if (m_keyboard.IsKeyHeld(SDL_SCANCODE_D)) {
         pos.x -= (float)sin(rot) * moveSpeed * delta;
         pos.z += (float)cos(rot) * moveSpeed * 1.0f * delta;
      } else if (m_keyboard.IsKeyHeld(SDL_SCANCODE_A)) {
         pos.x += (float)sin(rot) * moveSpeed * delta;
         pos.z += (float)cos(rot) * moveSpeed * -1.0f * delta;
      }

      if (m_keyboard.IsKeyHeld(SDL_SCANCODE_SPACE)) {
         pos.y+=5.5f*delta;
      } else if (m_keyboard.IsKeyHeld(SDL_SCANCODE_LSHIFT)) {
         pos.y-=5.5f*delta;
      }

      m_camera.SetPosition(pos);
   }

   void OnRender(float delta) override {
      m_directional.UseLight(m_ecs.GetComponent<Crimson::ModelComponent>(m_monkey)->shader.GetUniformLocation("directionalLight.ambientIntensity"),
                             m_ecs.GetComponent<Crimson::ModelComponent>(m_monkey)->shader.GetUniformLocation("directionalLight.color"),
                             m_ecs.GetComponent<Crimson::ModelComponent>(m_monkey)->shader.GetUniformLocation("directionalLight.diffuseIntensity"),
                             m_ecs.GetComponent<Crimson::ModelComponent>(m_monkey)->shader.GetUniformLocation("directionalLight.direction"));

      //m_shader.SetUniform3f("eyePosition", m_camera.GetPosition().x, m_camera.GetPosition().y, m_camera.GetPosition().z);
      m_shinyMaterial.UseMaterial(m_ecs.GetComponent<Crimson::ModelComponent>(m_monkey)->shader.GetUniformLocation("material.specularIntensity"),
                                  m_ecs.GetComponent<Crimson::ModelComponent>(m_monkey)->shader.GetUniformLocation("material.shininess"));
      Crimson::RenderModels(m_ecs, m_camera);

      m_gui.Render(GetSDLWindow());
   }
};

int main(int argc, char const *argv[]) {
   App app;
   app.Run();

   return 0;
}
