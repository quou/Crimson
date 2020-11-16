#include "AssetManagerPanel.h"

#include <filesystem>

#include "../Editor.h"
#include "../EditorLayer.h"

#include "../FontAwesome.h"

static bool CompareDir(DirectoryEntry a, DirectoryEntry b) {
    return a.isDirectory;
}

static std::vector<DirectoryEntry> GetFiles(const std::string& directory) {
	std::vector<DirectoryEntry> result;

	for (const auto& entry : std::filesystem::directory_iterator(directory)) {
		DirectoryEntry e = {entry.path().stem().string(), entry.path().extension().string(), entry.path().string(), entry.is_directory()};

		if (entry.is_directory()) {
			e.subEntries = GetFiles(entry.path().string());
		}

		result.push_back(e);
	}

	std::sort(result.begin(), result.end(), CompareDir);

	return result;
}

AssetManagerPanel::AssetManagerPanel(EditorLayer* editorLayer) : m_editorLayer(editorLayer) {
	m_files = GetFiles("Data/");
}

void AssetManagerPanel::DrawDir(DirectoryEntry& entry, Editor* editor, SceneHierarchyPanel& sceneHierarchyPanel) {
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

	if (!entry.isDirectory) {
		flags |= ImGuiTreeNodeFlags_Leaf;
	}

	std::string nodeString;

	if (entry.extension == ".as" || entry.extension == ".glsl") {
		nodeString = ICON_FK_CODE + std::string("  ") + entry.name + entry.extension;
	} else if (entry.extension == ".mesh") {
		nodeString = ICON_FK_CUBE + std::string("  ") + entry.name + entry.extension;
	} else if (entry.extension == ".cscn") {
		nodeString = ICON_FK_ARCHIVE + std::string("  ") + entry.name + entry.extension;
	} else if (entry.extension == ".ttf") {
		nodeString = ICON_FK_FONT + std::string("  ") + entry.name + entry.extension;
	} else if (entry.extension == ".conf") {
		nodeString = ICON_FK_COG + std::string("  ") + entry.name + entry.extension;
	} else if (entry.extension == ".mat") {
		nodeString = ICON_FK_CIRCLE + std::string("  ") + entry.name + entry.extension;
	} else if (entry.extension == ".png" || entry.extension == ".jpg" || entry.extension == ".gif" || entry.extension == ".bmp" || entry.extension == ".psd") {
		nodeString = ICON_FK_FILE_IMAGE_O + std::string("  ") + entry.name + entry.extension;
	} else if (entry.isDirectory) {
		nodeString = ICON_FK_FOLDER + std::string("  ") + entry.name;
	} else {
		nodeString = entry.name + entry.extension;
	}

	if (ImGui::TreeNodeEx(nodeString.c_str(), flags)) {
		if (entry.isDirectory) {
			for (auto& d : entry.subEntries) {
				DrawDir(d, editor, sceneHierarchyPanel);
			}
		} else {
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
            std::string dragString = entry.absPath;
            std::replace(dragString.begin(), dragString.end(), '\\', '/');

            ImGui::Text("%s", entry.absPath.c_str());

            ImGui::SetDragDropPayload(entry.extension.c_str(), dragString.data(), dragString.size() + 1);

            ImGui::EndDragDropSource();
         }

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
				if (entry.extension == ".cscn") {
					m_editorLayer->m_currentSavePath = entry.absPath;

					editor->m_scene = std::make_shared<Crimson::Scene>(false);
					sceneHierarchyPanel.SetContext(&*editor->m_scene);
					sceneHierarchyPanel.SetSelectionContext(Crimson::Entity());

					Crimson::SceneSerialiser sceneSerialiser(*editor->m_scene);
					sceneSerialiser.DeserialiseText(editor->m_scene->m_assetManager.LoadText(entry.absPath));
				}
			}
		}

		ImGui::TreePop();
	}
}

void AssetManagerPanel::Render(Editor* editor, SceneHierarchyPanel& sceneHierarchyPanel) {
	ImGui::Begin("Asset Manager");

	if (ImGui::Button("Refresh")) {
		m_files = GetFiles("Data/");
		Editor* editor = (Editor*)m_editorLayer->m_userData;
	}

	for (auto& f : m_files) {
		DrawDir(f, editor, sceneHierarchyPanel);
	}
	ImGui::End();
}
