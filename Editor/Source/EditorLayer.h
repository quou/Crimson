#pragma once

#include <Crimson.h>

#include "Panels/SceneHierarchyPanel.h"

class EditorLayer : public Crimson::Layer {
private:
	Crimson::Scene* m_scene;

	Crimson::RenderTarget* m_renderTarget;

	// Panels
	SceneHierarchyPanel m_sceneHierarchyPanel;
public:
	EditorLayer(Crimson::RenderTarget* renderTarget, Crimson::Scene* scene);

	void OnInit() override;
	void OnUpdate(float delta) override;
};
