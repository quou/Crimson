#include "AssetManagerPanel.h"

#include <filesystem>

#include "../Editor.h"

static std::vector<DirectoryEntry> GetFiles(const std::string& directory) {
	std::vector<DirectoryEntry> result;

	for (const auto& entry : std::filesystem::directory_iterator(directory)) {
		DirectoryEntry e = {entry.path().stem().string(), entry.path().extension().string(), entry.path().string(), entry.is_directory()};

		if (entry.is_directory()) {
			e.subEntries = GetFiles(entry.path().string());
		}

		result.push_back(e);
	}

	return result;
}

AssetManagerPanel::AssetManagerPanel() {
	m_files = GetFiles("Data/");
}

static void DrawDir(DirectoryEntry& entry, Editor* editor, SceneHierarchyPanel& sceneHierarchyPanel) {
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

	if (!entry.isDirectory) {
		flags |= ImGuiTreeNodeFlags_Leaf;
	}

	if (ImGui::TreeNodeEx(std::string(entry.name + entry.extension).c_str(), flags)) {
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
					editor->m_scene = std::make_shared<Crimson::Scene>(false);
					sceneHierarchyPanel.SetContext(&*editor->m_scene);
					sceneHierarchyPanel.SetSelectionContext(Crimson::Entity());

					Crimson::SceneSerialiser sceneSerialiser(*editor->m_scene);
					sceneSerialiser.DeserialiseText(editor->m_scene->m_assetManager.LoadText(entry.absPath));

					editor->m_scene->Init();
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
	}

	for (auto& f : m_files) {
		DrawDir(f, editor, sceneHierarchyPanel);
	}
	ImGui::End();
}
