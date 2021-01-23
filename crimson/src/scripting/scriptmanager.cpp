#include <angelscript/angelscript.h>

#include "scriptmanager.h"
#include "scriptregister.h"
#include "assets.h"

namespace Crimson {
	static const char* CrimsonDefaultLibrary = R"(
abstract class CrimsonBehaviour {
	Entity@ entity;
	
	void OnInit() {}
	void OnUpdate() {}
};
	)";

	ScriptManager::ScriptManager() {
		m_engine = asCreateScriptEngine();

		/* Engine config */
		m_engine->SetEngineProperty(asEP_REQUIRE_ENUM_SCOPE, true);

		m_context = m_engine->CreateContext();
		ScriptRegister::Register(m_engine);
	}

	void ScriptManager::Compile(const char* moduleName) {
		m_compilationSuccess = false;
		
		m_module = m_engine->GetModule(moduleName, asGM_ALWAYS_CREATE);

		/* Add the Crimson default code */
		m_module->AddScriptSection("libCrimson", CrimsonDefaultLibrary);

		/* Iterate the resources and find files with the .as extension */
		for (auto& entry : AssetManager::GetDir()) {
			if (entry.second == "as") {
				m_module->AddScriptSection(
					entry.first.c_str(), 
					AssetManager::LoadTerminatedString(
						entry.first.c_str()).c_str());
			}
		}

		int r = m_module->Build();
		if (r < 0) {
			/* Build errors */
			return;
		}

		m_compilationSuccess = true;
	}

	BehaviourInstance ScriptManager::CreateInstance(const char* classDecl, Entity* entity) {
		if (!m_compilationSuccess) { return BehaviourInstance{NULL, NULL, false}; }

		asITypeInfo* type = m_module->GetTypeInfoByDecl(classDecl);
		if (!type) {
			Log(LogType::ERROR, "Behaviour `%s' doesn't exist", classDecl);
			 return BehaviourInstance{NULL, NULL, false};
		}

		/* Build the factory declaration from via [classname]@ [classname]()
		 * eg. ExampleBehaviour@ ExampleBehaviour() */
		asIScriptFunction* factory = type->GetFactoryByDecl(
				(std::string(classDecl) + " @" + (classDecl) + "()").c_str());
		if (!factory) {
			Log(LogType::ERROR, "Behaviour `%s' doesn't exist", classDecl);
			return BehaviourInstance{NULL, NULL, false};
		}

		m_context->Prepare(factory);
		m_context->Execute();

		asIScriptObject* instance = 
			*(asIScriptObject**)m_context->GetAddressOfReturnValue(); /* Create the instance */
		
		/* Set the instance's entity property */
		Entity** e = (Entity**)instance->GetAddressOfProperty(0);
		(*e) = entity;

		instance->AddRef();

		return BehaviourInstance{type, instance, true};
	}

	void ScriptManager::CallInit(const BehaviourInstance& behaviour) {
		if (!m_compilationSuccess || !behaviour.isValid) { return; }

		asIScriptFunction* func = 
			behaviour.typeInfo->GetMethodByDecl("void OnInit()");
		if (!func) {
			return;
		}

		m_context->Prepare(func);
		m_context->SetObject(behaviour.instance);
		
		int r = m_context->Execute();
		if (r == asEXECUTION_EXCEPTION) {
			Log(LogType::ERROR, "Script exception: %s",
					m_context->GetExceptionString());
			return;
		}
	}
	
	void ScriptManager::CallUpdate(const BehaviourInstance& behaviour, float delta) {
		if (!m_compilationSuccess || !behaviour.isValid) { return; }

		asIScriptFunction* func = 
			behaviour.typeInfo->GetMethodByDecl("void OnUpdate(float)");
		if (!func) {
			return;
		}

		m_context->Prepare(func);
		m_context->SetArgFloat(0, delta);
		m_context->SetObject(behaviour.instance);
		
		int r = m_context->Execute();
		if (r == asEXECUTION_EXCEPTION) {
			Log(LogType::ERROR, "Script exception in %s::%s: %s",
					func->GetObjectName(), func->GetName(), m_context->GetExceptionString());
			return;
		}
	}

	ScriptManager::~ScriptManager() {
		m_context->Release();
		m_engine->ShutDownAndRelease();
	}
}
