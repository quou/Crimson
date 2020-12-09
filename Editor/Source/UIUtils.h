#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
void DrawQuatControl(const std::string& label, glm::quat& values, float colWidth = 100.0f);
void DrawTextControl(const std::string& label, std::string& string, float colWidth = 100.0f);
void DrawFloatControl(const std::string& label, float* val, float step = 1.0f, float min=0.0f, float max=0.0f, const std::string& tooltop = "", float colWidth = 100.0f);
void DrawIntControl(const std::string& label, int* val, int step=1, float colWidth = 100.0f);
void DrawBoolControl(const std::string& label, bool* val, float colWidth = 100.0f);
void DrawTextLabel(const std::string& label, const std::string& text, float colWidth = 100.0f);
void DrawColorControl(const std::string& label, glm::vec3& value, float colWidth = 100.0f);
void DrawLinePlot(const std::string& label, float* values, unsigned int valueLength, float valuesOffset, float min, float max, float colWidth=100.0f);

std::string DrawComboBox(const std::string& label, const std::string& defaultItem, const std::vector<std::string>& items, float colWidth=100.0f);
