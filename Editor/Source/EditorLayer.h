#pragma once

#include <Crimson.h>

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/AssetManagerPanel.h"

#include "SceneCamera.h"

class EditorLayer : public Crimson::Layer {
private:
	Crimson::RenderTarget* m_sceneRenderTarget;
	Crimson::RenderTarget* m_gameRenderTarget;

	// Panels
	SceneHierarchyPanel m_sceneHierarchyPanel;
	AssetManagerPanel m_assetManagerPanel;

	SceneCamera* m_camera;

	std::string m_currentSavePath;
	std::string m_currentSerialiseString;

	bool m_showProfiler{false};

	friend class AssetManagerPanel;
public:
	bool m_isRunning{false};

	EditorLayer(SceneCamera* sceneCamera, Crimson::RenderTarget* sceneRenderTarget, Crimson::RenderTarget* gameRenderTarget, Crimson::Scene* scene);

	void SaveAs();
	void SaveScene();
	void ReloadScene();
	void NewScene();

	void RunScene();
	void StopRunning();

	void OnInit() override;
	void OnUpdate(float delta) override;
};
