#include <assert.h>

#include <angelscript/angelscript.h>
#include <scriptbuilder/scriptbuilder.h>
#include <scriptstdstring/scriptstdstring.h>

#include "scriptmanager.h"
#include "logger.h"
#include "imguiregister.h"

namespace Crimson {
	static void MessageCallback(const asSMessageInfo* msg, void* param) {
		if (msg->type == asMSGTYPE_ERROR) {
			Log(LogType::ERROR, "%s (%d, %d): %s", msg->section, msg->row, msg->col, msg->message);
		} else if (msg->type == asMSGTYPE_WARNING) {
			Log(LogType::WARNING, "%s (%d, %d): %s", msg->section, msg->row, msg->col, msg->message);
		} else if (msg->type == asMSGTYPE_INFORMATION) {
			Log(LogType::INFO, "%s (%d, %d): %s", msg->section, msg->row, msg->col, msg->message);
		}
	}

	static void scriptPrint(std::string &msg)	{
		Log(LogType::INFO, "%s", msg.c_str());
	}

	ScriptManager::ScriptManager() {
		m_asEngine = asCreateScriptEngine();
		m_asContext = m_asEngine->CreateContext();
		m_asEngine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);

		RegisterStdString(m_asEngine);
		ASRegisterImGui(m_asEngine);

		int r;

		r = m_asEngine->RegisterGlobalFunction("void print(const string& in)", asFUNCTION(scriptPrint), asCALL_CDECL); assert(r >= 0);
	}

	ScriptManager::~ScriptManager() {
		m_asContext->Release();
		m_asEngine->ShutDownAndRelease();
	}

	void ScriptManager::Compile(const char* moduleName) {
		CScriptBuilder builder;

		int r = builder.StartNewModule(m_asEngine, moduleName);
		if (r < 0) {
			Log(LogType::ERROR, "Unrecoverable error starting new AngelScript module");
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
			Flush();
			return;
		}

		m_asModule = m_asEngine->GetModule(moduleName);

		m_compilationSuccess = true;

		Flush();
	}

	void ScriptManager::CallFunc(const char* decl) {
		if (!m_compilationSuccess) { return; }

		asIScriptFunction *func = m_asModule->GetFunctionByDecl(decl);

		m_asContext->Prepare(func);

		int r = m_asContext->Execute();
	}

	void ScriptManager::AddScript(const std::string& code) {
		m_codeFiles.push_back(code);
	}

	void ScriptManager::Flush() {
		m_codeFiles.clear();
	}
}