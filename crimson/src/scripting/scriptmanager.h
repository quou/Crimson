#pragma once

#include "core.h"

class asIScriptEngine;
class asIScriptModule;
class asITypeInfo;
class asIScriptContext;
class asIScriptObject;

namespace Crimson {
	class Entity;

	/* Container for the type info and the instance addresses
	 * of a behaviour. */
	struct BehaviourInstance {
		asITypeInfo* typeInfo;
		asIScriptObject* instance;
		bool isValid;
	};

	/* Responsible for compiling and running scripts */
	class CR_API ScriptManager {
	private:
		asIScriptEngine* m_engine;
		asIScriptModule* m_module;
		asIScriptContext* m_context;

		bool m_compilationSuccess = false;
	public:
		ScriptManager();
		virtual ~ScriptManager();

		/* Compile all the scripts from the asset manager's working directorys */
		void Compile(const char* moduleName);

		/* Create an instance of a behaviour, given it's declaration */
		BehaviourInstance CreateInstance(const char* classDecl, Entity* entity);

		/* Call the script's OnInit function */
		void CallInit(const BehaviourInstance& behaviour);

		/* Call the script's OnUpdate function */
		void CallUpdate(const BehaviourInstance& behaviour, float delta);

		inline bool CompiliationSuccess() const { return m_compilationSuccess; }
	};
}
