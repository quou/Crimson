#pragma once

#include "../TextEditor.h"

#include <imgui.h>

class CodeEditorPanel {
private:
	TextEditor m_textEditor;

	ImFont* m_font;

	std::string m_currentFile;

	bool m_isFocused{false};
public:
	void Init();
	void Render();

	void OpenFile(const std::string& path, const std::string& extension=".as");
	void Save();

	inline bool IsFocused() {return m_isFocused;};
};
