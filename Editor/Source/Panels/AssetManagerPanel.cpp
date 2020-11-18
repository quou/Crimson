#include "AssetManagerPanel.h"

#include <filesystem>

#include <Utils/tinyfiledialogs.h>

#include "../Editor.h"
#include "../EditorLayer.h"

#include "../FontAwesome.h"


static const char* defaultScript = R"(#include "Crimson"

class Script : CrimsonBehaviour {
	// Called on the first frame
	void OnInit() {

	}

	// Called once per frame
	void OnUpdate(float delta) {

	}
};
)";

static const char* defaultMaterial = R"(-- Crimson Material
shader = "Data/Shaders/Standard.glsl"

albedo = "Data/GridTexture.png"

material = {
	 color = {1, 1, 1},
	 smoothness = 1,
	 shininess = 20
}
)";


static const char* defaultShader = R"(#begin VERTEX

#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoords;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
	gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0f);
}

#end VERTEX

#begin FRAGMENT

#version 330 core

out vec4 a_fragPos;

void main() {
	a_fragPos = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

#end FRAGMENT
)";


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

	//std::sort(result.begin(), result.end(), CompareDir);

	return result;
}

AssetManagerPanel::AssetManagerPanel(EditorLayer* editorLayer) : m_editorLayer(editorLayer) {
	m_files = GetFiles("Data/");
}

void AssetManagerPanel::DrawDir(DirectoryEntry& entry, Editor* editor, SceneHierarchyPanel& sceneHierarchyPanel, CodeEditorPanel& codeEditorPanel) {
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
				DrawDir(d, editor, sceneHierarchyPanel, codeEditorPanel);
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

				} else if (entry.extension == ".lua" ||
							  entry.extension == ".conf" ||
							  entry.extension == ".mat" ||
							  entry.extension == ".as" ||
						  	  entry.extension == ".glsl") {
					codeEditorPanel.OpenFile(entry.absPath, entry.extension);
				}
			}
		}

		ImGui::TreePop();
	}
}

void AssetManagerPanel::Render(Editor* editor, SceneHierarchyPanel& sceneHierarchyPanel, CodeEditorPanel& codeEditorPanel) {
	ImGui::Begin("Asset Manager", NULL, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar()) {
		if (ImGui::MenuItem("Refresh")) {
			m_files = GetFiles("Data/");
			Editor* editor = (Editor*)m_editorLayer->m_userData;
		}

		if (ImGui::BeginMenu("Create...")) {
			if (ImGui::MenuItem("Material")) {
				const char* const acceptedExtensions[] = {"*.mat"};
				const char* file = tinyfd_saveFileDialog("New Material", "Data/NewMaterial.mat", 1, acceptedExtensions, "Crimson Material Files");

				if (file) {
					FILE* handle = fopen(file, "w");
					if (handle) {
						fprintf(handle, "%s", defaultMaterial);
					}
					fclose(handle);
				}
			}

			if (ImGui::MenuItem("Script")) {
				const char* const acceptedExtensions[] = {"*.as"};
				const char* file = tinyfd_saveFileDialog("New Script", "Data/NewScript.as", 1, acceptedExtensions, "AngelScript Files");

				if (file) {
					FILE* handle = fopen(file, "w");
					if (handle) {
						fprintf(handle, "%s", defaultScript);
					}
					fclose(handle);
				}
			}

			if (ImGui::MenuItem("Shader")) {
				const char* const acceptedExtensions[] = {"*.glsl"};
				const char* file = tinyfd_saveFileDialog("New Shader", "Data/NewShader.glsl", 1, acceptedExtensions, "OpenGL Shader Files");

				if (file) {
					FILE* handle = fopen(file, "w");
					if (handle) {
						fprintf(handle, "%s", defaultShader);
					}
					fclose(handle);
				}
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Import...")) {
			if (ImGui::MenuItem("Mesh")) {
				const char* const acceptedExtensionsIn[] = {"*.obj"};
				const char* const acceptedExtensionsOut[] = {"*.obj"};
				const char* in = tinyfd_openFileDialog("Import Mesh", "Data/", 1, acceptedExtensionsIn, "Mesh files", 0);
				const char* out = tinyfd_saveFileDialog("Save Mesh", "Data/Mesh.mesh", 1, acceptedExtensionsOut, "Crimson Mesh Files");

				if (in && out) {
					Crimson::ConvertFromObj(in, out);
				}
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	for (auto& f : m_files) {
		DrawDir(f, editor, sceneHierarchyPanel, codeEditorPanel);
	}
	ImGui::End();
}
