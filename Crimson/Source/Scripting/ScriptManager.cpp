#include "ScriptManager.h"

#include <angelscript/angelscript.h>
#include <scriptstdstring/scriptstdstring.h>
#include <scriptbuilder/scriptbuilder.h>

#include <cassert>

#include "Logger.h"
#include "AssetManager.h"


namespace Crimson {
	/* A base class that behaviour scripts will inherit from */
	static const char* g_behaviourBase = R"(
		abstract class CrimsonBehaviour {
			void OnInit() {}
			void OnUpdate(float delta) {}
		}
	)";

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

	static std::string scriptToString(double val) {
		return std::to_string(val);
	}

	static int IncludeCallback(const char *include, const char *from, CScriptBuilder *builder, void *userParam) {
      if (std::string(include) == "Crimson" || std::string(include) == "Crimson.as") {
         builder->AddSectionFromMemory("Crimson", g_behaviourBase);
         return 0;
      }

		auto assetManager = (AssetManager*)userParam;

      return builder->AddSectionFromMemory(include, assetManager->LoadText(include).c_str());
   }

	ScriptManager::ScriptManager() {
		m_asEngine = asCreateScriptEngine();
		m_asContext = m_asEngine->CreateContext();
		m_asEngine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);

		RegisterStdString(m_asEngine);

		int r;

		r = m_asEngine->RegisterGlobalFunction("void print(const string& in)", asFUNCTION(scriptPrint), asCALL_CDECL); assert(r >= 0);
		r = m_asEngine->RegisterGlobalFunction("string to_string(double)", asFUNCTION(scriptToString), asCALL_CDECL); assert(r >= 0);
	}

	ScriptManager::~ScriptManager() {
		for (auto obj : m_objects) {
			obj.first->Release();
		}

		m_asContext->Release();
		m_asEngine->ShutDownAndRelease();
	}

	void ScriptManager::Compile(AssetManager& assetManager) {
		m_compilationSuccess = false;

		CScriptBuilder builder;

		builder.SetIncludeCallback(IncludeCallback, &assetManager);

		int r = builder.StartNewModule(m_asEngine, "CrimsonBehaviours");
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


		m_asModule = m_asEngine->GetModule("CrimsonBehaviours");

		m_objects.clear();
		for (const auto& c : m_classNames) {
			asITypeInfo* type = m_asModule->GetTypeInfoByDecl(c.c_str());
			if (!type) {
				CR_LOG_ERROR("Class '%s' doesn't exist in the current module", c.c_str());
				continue;
			}

			asIScriptFunction* factory = type->GetFactoryByDecl(std::string(c + " @" + c + "()").c_str());
			if (!factory) {
				CR_LOG_ERROR("Class '%s' doesn't exist in the current module", c.c_str());
				continue;
			}

			m_asContext->Prepare(factory);
			m_asContext->Execute();

			asIScriptObject* obj = *(asIScriptObject**)m_asContext->GetAddressOfReturnValue();
			obj->AddRef();

			m_objects.push_back({obj, type});
		}

		m_compilationSuccess = true;

		Flush();
	}

	void ScriptManager::Init() {
		if (!m_compilationSuccess) {return;}

		for (auto obj : m_objects) {
			asIScriptFunction* func = obj.second->GetMethodByDecl("void OnInit()");
			if (!func) {
				CR_LOG_WARNING("%s", "No 'void OnInit' function");
				continue;
			}

			m_asContext->Prepare(func);
			m_asContext->SetObject(obj.first);
			int r = m_asContext->Execute();

			if (r == asEXECUTION_EXCEPTION) {
				CR_LOG_ERROR("An exception '%s' occurred.", m_asContext->GetExceptionString());
				continue;
			}
		}
	}

	void ScriptManager::Update(float delta) {
		if (!m_compilationSuccess) {return;}

		for (auto obj : m_objects) {
			asIScriptFunction* func = obj.second->GetMethodByDecl("void OnUpdate(float)");
			if (!func) {
				CR_LOG_WARNING("%s", "No 'void OnUpdate(float)' function");
				continue;
			}

			m_asContext->Prepare(func);
			m_asContext->SetArgFloat(0, delta);
			m_asContext->SetObject(obj.first);
			int r = m_asContext->Execute();

			if (r == asEXECUTION_EXCEPTION) {
				CR_LOG_ERROR("An exception '%s' occurred.", m_asContext->GetExceptionString());
				continue;
			}
		}
	}

	void ScriptManager::AddBehaviour(const std::string& className) {
		m_classNames.push_back(className);
	}

	void ScriptManager::AddScript(const std::string& code) {
		m_codeFiles.push_back(code);
	}

	void ScriptManager::Flush() {
		m_codeFiles.clear();
		m_classNames.clear();
	}
}
