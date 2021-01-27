#include <vector>


#include "gui.h"

namespace Crimson {
	bool DrawVec3Control(const std::string& label, vec3& values, float resetValue, float columnWidth) {
		bool xEdited = false;
		bool yEdited = false;
		bool zEdited = false;

		ImGui::PushID(label.c_str());

		ImGui::Columns(2, NULL, false);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text("%s", label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		xEdited = ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		yEdited = ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		zEdited = ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1, NULL, false);

		ImGui::PopID();

		return xEdited || yEdited || zEdited;
	}

	void DrawTextControl(const std::string& label, std::string& string, float colWidth) {
		ImGui::PushID(label.c_str());

		ImGui::Columns(2, NULL, false);


		ImGui::SetColumnWidth(0, colWidth);
		ImGui::Text("%s", label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy(buffer, string.c_str());
		if (ImGui::InputText("##STRIN", buffer, sizeof(buffer))) {
			string = std::string(buffer);
		}

		ImGui::PopItemWidth();

		ImGui::Columns(1, NULL, false);

		ImGui::PopID();
	}

	void DrawFloatControl(const std::string& label, float* val, float step, float min, float max, const std::string& tooltip, float colWidth) {
		ImGui::PushID(label.c_str());

		ImGui::Columns(2, NULL, false);


		ImGui::SetColumnWidth(0, colWidth);
		ImGui::Text("%s", label.c_str());
		if (ImGui::IsItemHovered() && !tooltip.empty()) {
			ImGui::SetNextWindowSize(ImVec2(200.0f, 0.0f));

			ImGui::BeginTooltip();
			ImGui::TextWrapped("%s", tooltip.c_str());
			ImGui::EndTooltip();
		}
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
		ImGui::DragFloat("##FLIN", val, step, min, max);

		ImGui::PopItemWidth();

		ImGui::Columns(1, NULL, false);

		ImGui::PopID();
	}

	void DrawIntControl(const std::string& label, int* val, int step, float colWidth) {
		ImGui::PushID(label.c_str());

		ImGui::Columns(2, NULL, false);


		ImGui::SetColumnWidth(0, colWidth);
		ImGui::Text("%s", label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());

		ImGui::InputInt("##ILIN", val, step);

		ImGui::PopItemWidth();

		ImGui::Columns(1, NULL, false);

		ImGui::PopID();
	}


	void DrawBoolControl(const std::string& label, bool* val, float colWidth) {
		ImGui::PushID(label.c_str());

		ImGui::Columns(2, NULL, false);


		ImGui::SetColumnWidth(0, colWidth);
		ImGui::Text("%s", label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
		ImGui::Checkbox("##BIN", val);

		ImGui::PopItemWidth();

		ImGui::Columns(1, NULL, false);

		ImGui::PopID();
	}

	void DrawTextLabel(const std::string& label, const std::string& text, float colWidth) {
		ImGui::PushID(label.c_str());

		ImGui::Columns(2, NULL, false);

		ImGui::SetColumnWidth(0, colWidth);
		ImGui::Text("%s", label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
		ImGui::Text("%s", text.c_str());

		ImGui::PopItemWidth();

		ImGui::Columns(1, NULL, false);

		ImGui::PopID();
	}

	void DrawColorControl(const std::string& label, Crimson::vec3& value, float colWidth) {
		ImGui::PushID(label.c_str());

		ImGui::Columns(2, NULL, false);


		ImGui::SetColumnWidth(0, colWidth);
		ImGui::Text("%s", label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());

		float col[3] = { value.x, value.y, value.z };
		ImGui::ColorEdit3("##COLIN", col);
		value = vec3(col[0], col[1], col[2]);

		ImGui::PopItemWidth();

		ImGui::Columns(1, NULL, false);

		ImGui::PopID();
	}

	void DrawLinePlot(const std::string& label, float* values, unsigned int valueLength, float valuesOffset, float min, float max, float colWidth) {
		ImGui::PushID(label.c_str());

		ImGui::Columns(2, NULL, false);


		ImGui::SetColumnWidth(0, colWidth);
		ImGui::Text("%s", label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());

		{
			float average = 0.0f;
			for (int n = 0; n < valueLength; n++)
				average += values[n];
			average /= (float)valueLength;
			char overlay[32];
			sprintf(overlay, "avg %f", average);
			ImGui::PlotLines("##LINEPLOT", values, valueLength, valuesOffset, overlay, min, max, ImVec2(0, 80.0f));
		}

		ImGui::PopItemWidth();

		ImGui::Columns(1, NULL, false);

		ImGui::PopID();
	}
}