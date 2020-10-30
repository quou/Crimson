#include "ScriptManager.h"

#include <angelscript/angelscript.h>
#include <scriptstdstring/scriptstdstring.h>
#include <scriptbuilder/scriptbuilder.h>

#include <cassert>

#include "Logger.h"

namespace Crimson {
	static void MessageCallback(const asSMessageInfo* msg, void* param) {
		if (msg->type == asMSGTYPE_ERROR) {
			CR_LOG_ERROR("%s (%d, %d): %s", msg->section, msg->row, msg->col, msg->message);
		} else if (msg->type == asMSGTYPE_WARNING) {
			CR_LOG_WARNING("%s (%d, %d): %s", msg->section, msg->row, msg->col, msg->message);
		} else if (msg->type == asMSGTYPE_INFORMATION) {
			CR_LOG("%s (%d, %d): %s", msg->section, msg->row, msg->col, msg->message);
		}
	}

	static void scriptPrint(std::string &msg)	{
		CR_LOG("%s", msg.c_str());
	}

	ScriptManager::ScriptManager() {
		m_asEngine = asCreateScriptEngine();
		m_asEngine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);

		RegisterStdString(m_asEngine);

		int r = m_asEngine->RegisterGlobalFunction("void print(const string& in)", asFUNCTION(scriptPrint), asCALL_CDECL); assert(r >= 0);
	}

	ScriptManager::~ScriptManager() {
		m_asEngine->ShutDownAndRelease();
	}

	void ScriptManager::Compile() {
		CScriptBuilder builder;
		int r = builder.StartNewModule(m_asEngine, "MyModule");
		if (r < 0) {
			CR_LOG_ERROR("%s", "Unrecoverable error starting new AngelScript module");
			return;
		}

		int i = 0;
		for (auto& s : m_codeFiles) {
			i++;

			int r = builder.AddSectionFromMemory(std::to_string(i).c_str(), s.c_str());
			if (r < 0) {
				continue;
			}
		}

		r = builder.BuildModule();
		if (r < 0) {
			return;
		}

		Flush();
	}

	void ScriptManager::Init() {
		asIScriptModule* mod = m_asEngine->GetModule("MyModule");
		asIScriptFunction* func = mod->GetFunctionByDecl("void OnInit()");
		if (func == NULL) {
			return;
		}

		asIScriptContext* context = m_asEngine->CreateContext();

		context->Prepare(func);
		int r = context->Execute();
		if (r == asEXECUTION_EXCEPTION) {
			CR_LOG_ERROR("An exception '%s' occurred", context->GetExceptionString());
		}

		context->Release();
	}

	void ScriptManager::Update(float delta) {

	}

	void ScriptManager::AddScript(const std::string& code) {
		m_codeFiles.push_back(code);
	}

	void ScriptManager::Flush() {
		m_codeFiles.clear();
	}
}
