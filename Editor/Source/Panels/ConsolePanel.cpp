#include "ConsolePanel.h"

#include "../UIUtils.h"
#include "../FontAwesome.h"

#include <imgui.h>

#include <Crimson.h>

void ConsolePanel::Render() {
	ImGui::Begin("Console", NULL, ImGuiWindowFlags_MenuBar);

	ImGui::BeginMenuBar();

	if (ImGui::MenuItem("Clear")) {
		Crimson::FlushLog();
		m_oldSize = 0;
	}

	ImGui::EndMenuBar();

	std::string lastString;

	for (auto& item : Crimson::GetLog()) {
		if (lastString == item.second) {
			continue;
		}
		lastString = item.second;

		const char* icon;
		ImVec4 color;

		switch (item.first) {
		case CR_LOGTYPE_INFO:
			icon = ICON_FK_INFO;
			color = ImVec4(0.7, 1, 0.7, 1);
			break;
		case CR_LOGTYPE_ERROR:
			icon = ICON_FK_EXCLAMATION_TRIANGLE;
			color = ImVec4(1, 0.6, 0.6, 1);
			break;
		case CR_LOGTYPE_FATAL_ERROR:
			icon = ICON_FK_EXCLAMATION_TRIANGLE;
			color = ImVec4(1, 0.4, 0.4, 1);
			break;
		case CR_LOGTYPE_WARNING:
			icon = ICON_FK_EXCLAMATION_CIRCLE;
			color = ImVec4(1, 0.5, 0.1, 1);
			break;
		default:
			break;
		}

		ImGui::PushStyleColor(ImGuiCol_Text, color);
		ImGui::Text(std::string(std::string(icon) +  "\t%s").c_str(), item.second.c_str());
		ImGui::PopStyleColor();

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
			ImGui::SetClipboardText(item.second.c_str());
		}

		ImGui::Separator();
	}

	if (Crimson::GetLog().size() > m_oldSize) {
		m_oldSize = Crimson::GetLog().size();
		ImGui::SetScrollY(ImGui::GetScrollMaxY() + 100);
	}

	ImGui::End();
}
