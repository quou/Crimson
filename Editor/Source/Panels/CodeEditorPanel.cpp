#include "CodeEditorPanel.h"

#include "../UIUtils.h"

#include <fstream>

#include "../Fonts/Roboto-Mono.h"

void CodeEditorPanel::Init() {
	ImGuiIO& io = ImGui::GetIO();
	m_font = io.Fonts->AddFontFromMemoryCompressedTTF(RobotoMono_compressed_data, RobotoMono_compressed_size, 20.0f);

	auto lang = TextEditor::LanguageDefinition::AngelScript();
	m_textEditor.SetLanguageDefinition(lang);
	m_textEditor.SetShowWhitespaces(false);
}

void CodeEditorPanel::Render(float delta) {
	ImGui::Begin(std::string("Code Editor - " + m_currentFile + "###Code Editor").c_str());

	ImGui::PushFont(m_font);
	m_textEditor.Render("CodeEditor");
	ImGui::PopFont();

	m_isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

	ImGui::End();

	m_lintCount += delta;

	if (m_lintCount > 1.0f && m_isFocused) {
		auto linterMessages = m_linter.Lint(m_textEditor.GetText());

		TextEditor::ErrorMarkers markers;
		for (auto& m : linterMessages) {
			markers[m.line] += m.message + "\n";
		}
		m_textEditor.SetErrorMarkers(markers);

		m_lintCount = 0.0f;
	}
}

void CodeEditorPanel::OpenFile(const std::string& path, const std::string& extension) {
	TextEditor::LanguageDefinition lang;
	if (extension == ".as") {
		lang = TextEditor::LanguageDefinition::AngelScript();
	} else if (extension == ".mesh" ||
				  extension == ".conf" ||
				  extension == ".mat" ||
				  extension == ".lua"
	) {
		lang = TextEditor::LanguageDefinition::Lua();
	} else if (extension == ".glsl") {
		lang = TextEditor::LanguageDefinition::GLSL();
	}

	m_textEditor.SetLanguageDefinition(lang);

	m_currentFile = path;

	std::ifstream t(path);
	if (t.good()) {
		std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		m_textEditor.SetText(str);
	}

	ImGui::SetWindowFocus(std::string("Code Editor - " + m_currentFile + "###Code Editor").c_str());
}

void CodeEditorPanel::Save() {
	std::ofstream out(m_currentFile);

	std::string text = m_textEditor.GetText();

	if (out.good()) {
		out << m_textEditor.GetText();
	}
}
