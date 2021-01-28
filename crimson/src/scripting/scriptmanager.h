#pragma once

#include <string>
#include <vector>

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
	struct CR_API BehaviourInstance {
		asITypeInfo* typeInfo;
		asIScriptObject* instance;
		bool isValid;
	};

	/* Contains information about a feild on a behaviour */
	struct CR_API BehaviourFeild {
		std::string name;
		int typeID;
		unsigned int index;

		bool operator<(const BehaviourFeild& f) const {
			return name < f.name; 
		}
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

		bool CheckBehaviourExistance(const char* decl);

		/* Recompile scripts if the asset manager has reloaded the text files.
		 * Returns true if changes where detected */
		bool HotReload();
		
		/* Get a vector of all the feilds for a behaviour */
		std::vector<BehaviourFeild> GetBehaviourFeilds(const BehaviourInstance& behaviour);

		bool IsFloat(const BehaviourFeild& feild);
		float GetFloatProperty(const BehaviourInstance& instance, const BehaviourFeild& feild);
		bool SetFloatProperty(const BehaviourInstance& instance, const BehaviourFeild& feild, float value);

		bool IsInt(const BehaviourFeild& feild);
		int GetIntProperty(const BehaviourInstance& instance, const BehaviourFeild& feild);
		bool SetIntProperty(const BehaviourInstance& instance, const BehaviourFeild& feild, int value);

		int GetStringTypeID();
		bool IsString(const BehaviourFeild& feild);
		std::string GetStringProperty(const BehaviourInstance& instance, const BehaviourFeild& feild);
		bool SetStringProperty(const BehaviourInstance& instance, const BehaviourFeild& feild, std::string value);

		inline bool CompiliationSuccess() const { return m_compilationSuccess; }
	};
}
