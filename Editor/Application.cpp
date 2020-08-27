#include <Crimson.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>


class App : public Crimson::Application {
private:
   Crimson::Shader m_shader;
   Crimson::Model m_model;
   Crimson::Texture m_texture;
   Crimson::DirectionalLight m_directional;
   Crimson::Camera m_camera;

   Crimson::Material m_shinyMaterial;

   glm::mat4 m_modl;
   glm::mat4 m_view;

   float m_curAngle = 0.0f;
public:
   App() :
      m_camera(glm::vec3(0,0,-5), 45.0f, 1366/768, 0.1f, 100.0f),
      m_model("Resources/monkey3.obj"),
      m_texture("Resources/Wood.jpg"),
      m_shader("Resources/Basic.vert", "Resources/Basic.frag"),
      m_shinyMaterial(1.0f, 32),
      m_directional(glm::vec3(1,1,1), 0.2f, 0.9f, glm::vec3(1,1,0)) {}

   void OnBegin() override  {
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGuiIO& io = ImGui::GetIO(); (void)io;
      io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
      io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

      ImGuiStyle& style = ImGui::GetStyle();
      if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
      {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
      }

      ImGui_ImplSDL2_InitForOpenGL(GetSDLWindow(), GetSDLGLContext());
      ImGui_ImplOpenGL3_Init("#version 330 core");

      m_texture.Bind(0);

      m_modl = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
      m_modl = glm::rotate(m_modl, 5.5f, glm::vec3(0, 1, 0));
      m_modl = glm::scale(m_modl, glm::vec3(1.0f, 1.0f, 1.0f));

      m_shader.Bind();
      m_shader.SetUniform1i("tex", 0);
      m_shader.SetUniformMatrix4("modl", m_modl);

      m_directional.UseLight(m_shader.GetUniformLocation("directionalLight.ambientIntensity"),
                             m_shader.GetUniformLocation("directionalLight.color"),
                             m_shader.GetUniformLocation("directionalLight.diffuseIntensity"),
                             m_shader.GetUniformLocation("directionalLight.direction"));

      m_shader.SetUniform3f("eyePosition", m_camera.GetPosition().x, m_camera.GetPosition().y, m_camera.GetPosition().z);
      m_shinyMaterial.UseMaterial(m_shader.GetUniformLocation("material.specularIntensity"),
                                  m_shader.GetUniformLocation("material.shininess"));
   }

   void OnUpdate(float delta) override {
      ImGui_ImplSDL2_ProcessEvent(&GetEvent());


      m_camera.UpdatePerspective(45.0f, (float)GetDisplay()->GetSize().first/(float)GetDisplay()->GetSize().second, 0.1f, 100.0f);
      m_shader.SetUniformMatrix4("view", m_camera.GetViewProjection());
      m_shader.SetUniform3f("eyePosition", m_camera.GetPosition().x, m_camera.GetPosition().y, m_camera.GetPosition().z);

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
      m_shader.SetUniformMatrix4("modl", m_modl);
      m_model.Render();

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplSDL2_NewFrame(GetSDLWindow());
      ImGui::NewFrame();

      ImGui::ShowDemoWindow();

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      ImGuiIO& io = ImGui::GetIO(); (void)io;
      if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
         SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
         SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
         ImGui::UpdatePlatformWindows();
         ImGui::RenderPlatformWindowsDefault();
         SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
      }
   }
};

int main(int argc, char const *argv[]) {
   App app;
   app.Run();

   ImGui_ImplOpenGL3_Shutdown();
   ImGui_ImplSDL2_Shutdown();
   ImGui::DestroyContext();

   return 0;
}
