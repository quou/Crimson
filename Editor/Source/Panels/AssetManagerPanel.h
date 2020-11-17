#pragma once

#include <Crimson.h>

#include "SceneHierarchyPanel.h"
#include "CodeEditorPanel.h"

class Editor;
class EditorLayer;

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

	EditorLayer* m_editorLayer;
public:
	AssetManagerPanel(EditorLayer* editorLayer);

	void DrawDir(DirectoryEntry& entry, Editor* editor, SceneHierarchyPanel& sceneHierarchyPanel, CodeEditorPanel& codeEditorPanel);
	void Render(Editor* editor, SceneHierarchyPanel& sceneHierarchyPanel, CodeEditorPanel& codeEditorPanel);
};
