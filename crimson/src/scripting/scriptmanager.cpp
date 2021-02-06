#include <angelscript/angelscript.h>

#include "scriptmanager.h"
#include "scriptregister.h"
#include "assets.h"

namespace Crimson {
	static const char* CrimsonDefaultLibrary = R"(
abstract class CrimsonBehaviour {
	protected Entity@ entity;
	
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
				std::string src = AssetManager::LoadTerminatedString(entry.first.c_str(), true);
				m_module->AddScriptSection(
					entry.first.c_str(), src.c_str());
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
		/* Incase the user wants a script component that isn't attached to a behaviour */
		if (std::string(classDecl) == "~") {
			return BehaviourInstance{NULL, NULL, false}; 
		}

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

	bool ScriptManager::CheckBehaviourExistance(const char* decl) {
		if (!m_compilationSuccess) { return false; }

		asITypeInfo* type = m_module->GetTypeInfoByDecl(decl);
	
		return type == NULL ? false : true;
	}

	bool ScriptManager::HotReload() {
		for (auto& entry : AssetManager::GetDir()) {
			if (entry.second == "as" && AssetManager::TextFileModified(entry.first.c_str())) {
				Compile(m_module->GetName());
				return true; /* All scripts get recompiled, so no point looping over any others */
			}
		}

		return false;
	}

	std::vector<BehaviourField> ScriptManager::GetBehaviourFields(const BehaviourInstance& behaviour) {
		std::vector<BehaviourField> result;

		if (!behaviour.instance || !behaviour.typeInfo) { return result; }

		for (unsigned int i = 0; i < behaviour.instance->GetPropertyCount(); i++) {
			bool isPrivate;
			bool isProtected;
			int typeID;

			behaviour.typeInfo->GetProperty(i, NULL, &typeID, &isPrivate, &isProtected);

			if (!isPrivate && !isProtected) {
				result.push_back(BehaviourField{
					behaviour.instance->GetPropertyName(i),
					typeID,
					i
				});
			}
		}

		return result;
	}

	std::vector<std::string> ScriptManager::GetBehaviourNames() {
		std::vector<std::string> result;

		if (!m_compilationSuccess) { return result; }

		asITypeInfo* baseTypeInfo = m_module->GetTypeInfoByDecl("CrimsonBehaviour");

		for (unsigned int i = 0; i < m_module->GetObjectTypeCount(); i++) {
			asITypeInfo* ti = m_module->GetObjectTypeByIndex(i);

			if (ti && ti->DerivesFrom(baseTypeInfo) && ti != baseTypeInfo) {
				result.push_back(ti->GetName());
			}
		}

		return result;
	}

	bool ScriptManager::IsFloat(const BehaviourField& field) {
		return field.typeID == asTYPEID_FLOAT;
	}

	float ScriptManager::GetFloatProperty(const BehaviourInstance& instance, const BehaviourField& field) {
		if (!instance.instance) {
			return 0.0f;
		}

		void* prop = instance.instance->GetAddressOfProperty(field.index);

		if (!prop) {
			return 0.0f;
		}

		return *(float*)prop;
	}

	bool ScriptManager::SetFloatProperty(const BehaviourInstance& instance, const BehaviourField& field, float value) {
		if (!instance.instance || field.typeID == -1) {
			return false;
		}

		void* prop = instance.instance->GetAddressOfProperty(field.index);
		if (!prop) {
			return false;
		}

		float* v = (float*)prop;
		(*v) = value;

		return true;
	}

	bool ScriptManager::IsInt(const BehaviourField& field) {
		return field.typeID == asTYPEID_INT32;
	}

	int ScriptManager::GetIntProperty(const BehaviourInstance& instance, const BehaviourField& field) {
		if (!instance.instance) {
			return 0;
		}

		void* prop = instance.instance->GetAddressOfProperty(field.index);

		if (!prop) {
			return 0;
		}

		return *(int*)prop;
	}

	bool ScriptManager::SetIntProperty(const BehaviourInstance& instance, const BehaviourField& field, int value) {
		if (!instance.instance || field.typeID == -1) {
			return false;
		}

		void* prop = instance.instance->GetAddressOfProperty(field.index);
		if (!prop) {
			return false;
		}

		int* v = (int*)prop;
		(*v) = value;

		return true;
	}
	
	int ScriptManager::GetStringTypeID() {
		return m_engine->GetTypeIdByDecl("string");
	}

	bool ScriptManager::IsString(const BehaviourField& field) {
		return field.typeID == m_engine->GetTypeIdByDecl("string");
	}

	std::string ScriptManager::GetStringProperty(const BehaviourInstance& instance, const BehaviourField& field) {
		if (!instance.instance) {
			return 0;
		}

		void* prop = instance.instance->GetAddressOfProperty(field.index);

		if (!prop) {
			return 0;
		}

		return *(std::string*)prop;
	}

	bool ScriptManager::SetStringProperty(const BehaviourInstance& instance, const BehaviourField& field, std::string value) {
		if (!instance.instance || field.typeID == -1) {
			return false;
		}

		void* prop = instance.instance->GetAddressOfProperty(field.index);
		if (!prop) {
			return false;
		}

		std::string* v = (std::string*)prop;
		(*v) = value;

		return true;
	}

	ScriptManager::~ScriptManager() {
		m_context->Release();
		m_engine->ShutDownAndRelease();
	}
}
