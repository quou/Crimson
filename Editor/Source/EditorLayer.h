#pragma once

#include <Crimson.h>

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/AssetManagerPanel.h"

#include "SceneCamera.h"

class EditorLayer : public Crimson::Layer {
private:
	Crimson::RenderTarget* m_renderTarget;

	// Panels
	SceneHierarchyPanel m_sceneHierarchyPanel;
	AssetManagerPanel m_assetManagerPanel;

	SceneCamera* m_camera;

	std::string m_currentSavePath;

	friend class AssetManagerPanel;
public:
	EditorLayer(SceneCamera* sceneCamera, Crimson::RenderTarget* renderTarget, Crimson::Scene* scene);

	void SaveAs();
	void SaveScene();
	void ReloadScene();
	void NewScene();

	void OnInit() override;
	void OnUpdate(float delta) override;
};
