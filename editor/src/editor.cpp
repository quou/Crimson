#include <crimson.h>
#include <imgui.h>

#include "panel.h"
#include "heirarchy.h"
#include "viewport.h"

namespace Crimson {
	class Editor : public Application {
	private:
		ref<Scene> m_scene;
		Entity* ent;
		Entity* pointLight;
		Entity* pointLight2;
		
		Camera m_camera;

		ref<PanelManager> m_panelManager;
	public:
		void OnInit() override {
			ImGuiManager::Init(m_window);

			/* Create panels */
			m_panelManager = ref<PanelManager>(new PanelManager());
			m_panelManager->AddPanel(ref<Panel>(new Heirarchy()));
			m_panelManager->AddPanel(ref<Panel>(new Viewport()));

			/* Create the scene */
			m_scene = ref<Scene>(new Scene());

			/* Create a sphere model */
			// ref<Material> material(new PhongMaterial("standard.glsl", vec3(1.0f, 1.0f, 1.0f), 32.0f));
			// ref<Model> model(new Model());
			// model->AddMesh(MeshFactory::NewSphereMesh(material));

			// /* Create the sphere entity */
			// ent = m_scene->CreateEntity("Sphere");
			// ent->AddComponent<TransformComponent>();
			// ent->AddComponent<RenderableComponent>(model);
			// ent->AddComponent<ScriptComponent>("TestBehaviour");

			// /* Create point light entities */
			// pointLight = m_scene->CreateEntity("Disco Light");
			// pointLight->AddComponent<TransformComponent>()->translation = vec3(5.0f, -1.0f, 4.0f);
			// pointLight->AddComponent<PointLightComponent>(vec3(1.0f), 1.0f);
			// pointLight->AddComponent<SkyLightComponent>(vec3(1.0f), 0.1f);
			// pointLight->AddComponent<ScriptComponent>("DiscoLight");

			/* Create the camera */
			m_camera = Camera(m_window->GetWidth(), m_window->GetHeight(), 70.0f, 0.1f, 100.0f);
			m_camera.position = vec3(0.0f, 0.5f, 5.0f);

			SceneSerialiser s(m_scene);
			s.DeserialiseScene("test.crimson");
		}

		void OnUpdate(float delta) override {
			m_scene->Update(delta);
			AssetManager::HotReload();

			Renderer::Clear(0.0f, 0.0f, 0.0f);

			/* Draw panels */
			ImGuiManager::BeginFrame();
			m_panelManager->Draw(m_camera, m_scene);
			ImGuiManager::EndFrame();
		}

		void OnExit() {
			ImGuiManager::Quit();
		}
	};

}

int main() {
	Crimson::Editor app;
	app.Run("Crimson Editor", 1366, 768);
}
