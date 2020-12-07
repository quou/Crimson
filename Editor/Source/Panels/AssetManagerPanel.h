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

	std::shared_ptr<Crimson::Texture> m_texturePreview;
	std::string m_texturePreviewPath;

	EditorLayer* m_editorLayer;

	friend class EditorLayer;
	friend class SceneHierarchyPanel;
public:
	AssetManagerPanel(EditorLayer* editorLayer);

	std::vector<DirectoryEntry> GetFiles(const std::string& directory);

	void DrawDir(DirectoryEntry& entry, Editor* editor, SceneHierarchyPanel& sceneHierarchyPanel, CodeEditorPanel& codeEditorPanel);
	void Render(Editor* editor, SceneHierarchyPanel& sceneHierarchyPanel, CodeEditorPanel& codeEditorPanel);

	void Refresh();
};
