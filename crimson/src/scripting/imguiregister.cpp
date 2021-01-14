#include <string>
#include <assert.h>

#include <angelscript/angelscript.h>
#include <imgui.h>

#include "imguiregister.h"

namespace Crimson {
	static void ImGuiBegin(std::string& in) {
		ImGui::Begin(in.c_str());
	}

	static void ImGuiText(std::string& in) {
		ImGui::Text("%s", in.c_str());
	}

	void ASRegisterImGui(asIScriptEngine* engine) {
		engine->SetDefaultNamespace("ImGui");

		int r;

		r = engine->RegisterGlobalFunction("void Begin(const string& in)", asFUNCTION(ImGuiBegin), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("void Text(const string& in)", asFUNCTION(ImGuiText), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("void End()", asFUNCTION(ImGui::End), asCALL_CDECL); assert(r >= 0);
	}
}