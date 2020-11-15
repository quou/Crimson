#include "ConsolePanel.h"

#include "../UIUtils.h"
#include "../FontAwesome.h"

#include <imgui.h>

#include <Crimson.h>

void ConsolePanel::Render() {
	ImGui::Begin("Console");

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

		ImGui::Separator();
	}

	ImGui::End();
}
