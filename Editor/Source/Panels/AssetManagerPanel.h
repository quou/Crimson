#pragma once

#include <Crimson.h>

#include "SceneHierarchyPanel.h"

class Editor;

struct DirectoryEntry {
	std::string name;
	std::string extension;
	std::string absPath;

	bool isDirectory;
	std::vector<DirectoryEntry> subEntries;
};

class AssetManagerPanel {
private:
	std::vector<DirectoryEntry> m_files;
public:
	AssetManagerPanel();
	void Render(Editor* editor, SceneHierarchyPanel& sceneHierarchyPanel);
};
