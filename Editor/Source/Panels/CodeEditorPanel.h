#pragma once

#include "../TextEditor.h"

#include <imgui.h>

#include <Crimson.h>

class CodeEditorPanel {
private:
	TextEditor m_textEditor;

	ImFont* m_font;

	std::string m_currentFile;

	bool m_isFocused{false};

	float m_lintCount{0.0f};
	Crimson::Linter m_linter;

	std::string m_unsavedText;

	std::string m_sourceType;
public:
	void Init();
	void Render(float delta);

	void OpenFile(const std::string& path, const std::string& extension=".as");
	void Save();

	inline bool IsFocused() {return m_isFocused;};
};
