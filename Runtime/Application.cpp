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
      Crimson::CompileScripts(m_ecs);
      Crimson::InstancePrefabs(m_ecs, m_sceneManager);
      Crimson::InitScripts(m_ecs);
   }

   void OnUpdate(float delta) override {
      Crimson::UpdateScripts(m_ecs, delta);
      Crimson::UpdateCameras(m_ecs, GetDisplay()->GetSize().first, GetDisplay()->GetSize().second);
   }

   void OnRender(float delta) override {
      Crimson::ShadowPass(m_ecs, m_sceneManager);
      GetDisplay()->BindAsRenderTarget();
      Crimson::RenderModels(m_ecs, m_sceneManager);
   }

   virtual ~App() {

   }
};


App app;

int main(int argc, char const *argv[]) {
   app.Run();

   return 0;
}
