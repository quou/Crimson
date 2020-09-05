#include <Crimson.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class App : public Crimson::Application {
private:
   Crimson::Camera m_camera;

   glm::mat4 m_modl;
   glm::mat4 m_view;

   float m_curAngle = 0.0f;
public:
   App() :
      m_camera(glm::vec3(0,0,-5), 45.0f, 800/600, 0.1f, 100.0f) {}

   void OnBegin() override  {
      m_sceneManager.Deserialize("Resources/TestScene.scene", m_ecs);
   }

   void OnUpdate(float delta) override {
      m_camera.UpdatePerspective(45.0f, (float)GetDisplay()->GetSize().first/(float)GetDisplay()->GetSize().second, 0.1f, 100.0f);

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
      Crimson::RenderModels(m_ecs, m_camera, m_sceneManager);
   }
};

int main(int argc, char const *argv[]) {
   App app;
   app.Run();

   return 0;
}
