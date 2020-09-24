#include <Crimson.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>

#include <SDL.h>
#undef main

class App : public Crimson::Application {
public:
   App() : Application("Crimson") {}

   void OnBegin() override {
      m_sceneManager.Deserialize("Resources/Scenes/TestScene.scene", m_ecs);
      m_sceneManager.MakeCameraCurrent();
   }

   void OnUpdate(float delta) override {
      m_cameraSystem->Update(m_ecs, GetDisplay()->GetSize().first, GetDisplay()->GetSize().second);
   }

   void OnRender(float delta) override {
      m_lightingSystem->Update(m_ecs);
      m_graphicsSystem->RenderShadows(m_ecs, m_sceneManager);
      GetDisplay()->BindAsRenderTarget();
      m_graphicsSystem->Render(m_ecs, m_sceneManager);
   }

   virtual ~App() {

   }
};


App app;

int main(int argc, char const *argv[]) {
   app.Run();

   return 0;
}
