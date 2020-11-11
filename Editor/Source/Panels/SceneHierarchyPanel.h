#pragma once

#include <Crimson.h>

class SceneHierarchyPanel {
private:
	Crimson::Scene* m_scene;

	Crimson::Entity m_selectedEntity;

	void DrawComponents(Crimson::Entity ent);
	void DrawEntityNode(Crimson::Entity ent);
public:
	SceneHierarchyPanel(Crimson::Scene* scene);

	inline void SetContext(Crimson::Scene* scene) {m_scene = scene;}

	void Render();
};
