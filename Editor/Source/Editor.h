#pragma once

#include <Crimson.h>

#include "EditorLayer.h"

#include "SceneCamera.h"

class Editor : public Crimson::Game {
public:
	std::shared_ptr<Crimson::Scene> m_scene;

	std::shared_ptr<Crimson::RenderTarget> m_gameRenderTarget;
	std::shared_ptr<Crimson::RenderTarget> m_sceneRenderTarget;

	std::shared_ptr<Crimson::ParticleSystem> m_particles;

	Crimson::Entity cam;
	Crimson::Entity mainLight;

	SceneCamera m_camera;
private:
	void OnInit() override {
		Crimson::Input::RegisterKey("shift", CR_KEY_LEFT_SHIFT);

		m_scene = std::make_shared<Crimson::Scene>(false);

		m_sceneRenderTarget = std::make_shared<Crimson::RenderTarget>(GetWindowSize());
		m_gameRenderTarget = std::make_shared<Crimson::RenderTarget>(GetWindowSize());

		m_particles = std::make_shared<Crimson::ParticleSystem>(glm::vec3(0.0f, 3.0f, 0.0f));
		m_particles->m_rateOverTime = 10.0f;

		AddLayer<EditorLayer>(this, &m_camera, m_sceneRenderTarget.get(), m_gameRenderTarget.get(), m_scene.get());
	}

	void OnUpdate(float delta) override {
		m_particles->Update(delta);

		m_camera.UpdateViewport(m_sceneRenderTarget->GetSize());

		m_scene->UpdateViewport(m_sceneRenderTarget->GetSize());
		m_scene->Render(*m_sceneRenderTarget, m_camera.GetCamera());

		m_particles->Draw(*m_camera.GetCamera());

		m_sceneRenderTarget->Unbind();

		m_scene->UpdateViewport(m_gameRenderTarget->GetSize());
		m_scene->Render(*m_gameRenderTarget);
		m_gameRenderTarget->Unbind();
   }

	void OnExit() override {
		m_particles.reset();
		m_sceneRenderTarget.reset();
		m_gameRenderTarget.reset();
		m_scene.reset();
	}
public:
};
