#include "AssetManagerPanel.h"

#include <filesystem>

static std::vector<DirectoryEntry> GetFiles(const std::string& directory) {
	std::vector<DirectoryEntry> result;

	for (const auto& entry : std::filesystem::directory_iterator(directory)) {
		DirectoryEntry e = {entry.path().stem().string(), entry.path().extension().string(), entry.is_directory()};

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

static void DrawDir(DirectoryEntry& entry) {
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

	if (!entry.isDirectory) {
		flags |= ImGuiTreeNodeFlags_Leaf;
	}

	if (ImGui::TreeNodeEx(std::string(entry.name + entry.extension).c_str(), flags)) {
		if (entry.isDirectory) {
			for (auto& d : entry.subEntries) {
				DrawDir(d);
			}
		}

		ImGui::TreePop();
	}
}

void AssetManagerPanel::Render() {
	ImGui::Begin("Asset Manager");

	if (ImGui::Button("Refresh")) {
		m_files = GetFiles("Data/");
	}

	for (auto& f : m_files) {
		DrawDir(f);
	}
	ImGui::End();
}
