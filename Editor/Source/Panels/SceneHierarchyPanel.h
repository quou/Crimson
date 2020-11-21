#pragma once

#include <Crimson.h>

class SceneHierarchyPanel {
private:
	Crimson::Scene* m_scene;

	Crimson::Entity m_selectedEntity;

	void DrawComponents(Crimson::Entity ent);
	void DrawEntityNode(Crimson::Entity ent);

	friend class EditorLayer;
public:
	SceneHierarchyPanel(Crimson::Scene* scene);

	inline void SetContext(Crimson::Scene* scene) {m_scene = scene;}
	inline void SetSelectionContext(Crimson::Entity ent) {m_selectedEntity = ent;}

	void Render();
};
