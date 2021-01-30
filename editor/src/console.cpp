#include <imgui.h>

#include "console.h"

namespace Crimson {
	void Console::OnDraw(const ref<Scene>& scene, float delta) {
		ImGui::Begin("console", NULL, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

		static bool hideInfo = false;
		static bool hideWarnings = false;
		static bool hideErrors = false;

		static std::string selectedEntry;

		if (ImGui::BeginMenuBar()) {
			if (ImGui::Button("clear")) {
				ClearLogs();
			}
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
			if (ImGui::Button(ICON_FK_INFO)) {
				hideInfo = !hideInfo;
			}
			ImGui::PopStyleColor();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.15f, 1.0f));
			if (ImGui::Button(ICON_FK_EXCLAMATION_TRIANGLE)) {
				hideWarnings = !hideWarnings;
			}
			ImGui::PopStyleColor();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
			if (ImGui::Button(ICON_FK_EXCLAMATION_CIRCLE)) {
				hideErrors = !hideErrors;
			}
			ImGui::PopStyleColor();

			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
			if (ImGui::Button(ICON_FK_CLIPBOARD)) {
				ImGui::SetClipboardText(selectedEntry.c_str());
			}
			ImGui::PopStyleColor();

			ImGui::EndMenuBar();
		}

		for (const LogEntry& entry : GetLogs()) {
			const char* icon;

			switch (entry.type) {
			case LogType::ERROR:
				if (hideErrors) { continue; }

				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
				icon = ICON_FK_EXCLAMATION_CIRCLE;
				break;
			case LogType::INFO:
				if (hideInfo) { continue; }

				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
				icon = ICON_FK_INFO;
				break;
			case LogType::WARNING:
				if (hideWarnings) { continue; }

				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.15f, 1.0f));
				icon = ICON_FK_EXCLAMATION_TRIANGLE;
				break;
			}


			ImGui::Selectable((std::string(icon) + " " + entry.message).c_str(), entry.message == selectedEntry);

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				ImGui::SetClipboardText(entry.message.c_str());
			}

			if (ImGui::IsItemClicked()) {
				selectedEntry = entry.message;
			}
		
			ImGui::PopStyleColor();
		}

		ImGui::End();
	}
}