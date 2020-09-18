#include <Crimson.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>

#include "GUI.h"

#include <SDL.h>
#undef main

class App : public Crimson::Application {
private:
   Crimson::Camera m_camera;

   glm::mat4 m_modl;
   glm::mat4 m_view;

   float m_curAngle {0.0f};

   GUI m_gui;

   Crimson::RenderTarget m_renderTarget;

   bool m_isPlaying {false};

public:
   App() : Application("Editor"),
      m_camera(glm::vec3(0,0,-5), 45.0f, 1366/768, 0.0f, 1000.0f) {}

   void OnBegin() override {
      std::cout << "Light ID: " <<  GetComponentID<Crimson::PointLight>() << '\n';
      std::cout << "Model ID: " <<  GetComponentID<Crimson::ModelComponent>() << '\n';
      std::cout << "Transform ID: " <<  GetComponentID<Crimson::Transform>() << '\n';
      std::cout << "Prefab ID: " <<  GetComponentID<Crimson::PrefabInstancerComponent>() << '\n';


      m_gui.Init(GetSDLWindow(), GetSDLGLContext());

      m_gui.OpenScene("Resources/Scenes/TestScene.scene", m_sceneManager, m_ecs);

      m_sceneManager.SetCurrentCamera(&m_camera);
   }

   void OnUpdate(float delta) override {
      if (m_gui.GetShouldPlay() != m_isPlaying) {
         if (m_gui.GetShouldPlay()) {
            Play();
         } else {
            Stop();
         }
         m_isPlaying = m_gui.GetShouldPlay();
      }

      if (m_isPlaying) {
         Crimson::UpdateScripts(m_ecs, delta);
      }

      Crimson::UpdateCameras(m_ecs, m_renderTarget);
      m_camera.UpdatePerspective(45.0f, (float)m_renderTarget.GetWidth()/(float)m_renderTarget.GetHeight(), 0.1f, 100.0f);

      float pitch = m_camera.GetPitch();
      float yaw = m_camera.GetYaw();

      float rotSpeed = 800.0f;
      float moveSpeed = 10.0f;

      float yoffset = 0.0f;
      float xoffset = 0.0f;

      if (m_gui.IsSceneFocused()) {
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
      if (m_gui.IsSceneFocused()) {
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
      }

      m_camera.SetPosition(pos);
   }

   void OnRender(float delta) override {
      Crimson::ShadowPass(m_ecs, m_sceneManager);
      m_renderTarget.Bind();
      m_renderTarget.Clear();
      Crimson::RenderModels(m_ecs, m_sceneManager);

      GetDisplay()->BindAsRenderTarget();
      m_gui.Render(GetSDLWindow(), m_ecs, m_sceneManager, m_camera, m_renderTarget);

      m_gui.EndFrame();
   }

   void Play() {
      std::cout << "play" << '\n';

      m_gui.SaveScene(m_sceneManager, m_ecs);

      if (m_gui.IsSaved()) {
         m_sceneManager.MakeCameraCurrent();

         Crimson::CompileScripts(m_ecs);
         Crimson::InstancePrefabs(m_ecs, m_sceneManager);
         Crimson::InitScripts(m_ecs);
      }
      m_isPlaying = true;
      ImGui::StyleColorsClassic();
   }

   void Stop() {
      std::cout << "stop" << '\n';


      m_gui.OpenScene(m_gui.GetCurrentScenePath(), m_sceneManager, m_ecs);

      m_sceneManager.SetCurrentCamera(&m_camera);

      m_isPlaying = false;
      ImGui::StyleColorsDark();
   }

   void UpdateGui(SDL_Event e) {
      m_gui.Update(e);
   }

   virtual ~App() {

   }
};


App app;

void EventCallback(SDL_Event event) {
   app.UpdateGui(event);
}

int main(int argc, char const *argv[]) {
   app.SetEventCallback(EventCallback);
   app.Run();

   return 0;
}
