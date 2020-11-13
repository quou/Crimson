#pragma once

#include <Crimson.h>

#include "EditorLayer.h"

#include "SceneCamera.h"

class Editor : public Crimson::Game {
public:
	std::shared_ptr<Crimson::Scene> m_scene;

	std::shared_ptr<Crimson::RenderTarget> m_gameRenderTarget;
	std::shared_ptr<Crimson::RenderTarget> m_sceneRenderTarget;

	Crimson::Entity cam;
	Crimson::Entity mainLight;

	SceneCamera m_camera;
private:
	void OnInit() override {
		Crimson::Input::RegisterKey("shift", CR_KEY_LEFT_SHIFT);

		m_scene = std::make_shared<Crimson::Scene>(false);

		m_sceneRenderTarget = std::make_shared<Crimson::RenderTarget>(GetWindowSize());
		m_gameRenderTarget = std::make_shared<Crimson::RenderTarget>(GetWindowSize());

		AddLayer<EditorLayer>(this, &m_camera, m_sceneRenderTarget.get(), m_gameRenderTarget.get(), m_scene.get());
	}

	void OnUpdate(float delta) override {
		m_camera.UpdateViewport(m_sceneRenderTarget->GetSize());

		m_scene->UpdateViewport(m_sceneRenderTarget->GetSize());
		m_scene->Render(*m_sceneRenderTarget, m_camera.GetCamera());
		m_scene->UpdateViewport(m_gameRenderTarget->GetSize());
		m_scene->Render(*m_gameRenderTarget);
   }

	void OnExit() override {
		m_sceneRenderTarget.reset();
		m_gameRenderTarget.reset();
		m_scene.reset();
	}
public:
};
