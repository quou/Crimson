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

	/* Contains information about a field on a behaviour */
	struct CR_API BehaviourField {
		std::string name;
		int typeID;
		unsigned int index;

		bool operator<(const BehaviourField& f) const {
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
		
		/* Get a vector of all the fields for a behaviour */
		std::vector<BehaviourField> GetBehaviourFields(const BehaviourInstance& behaviour);

		bool IsFloat(const BehaviourField& field);
		float GetFloatProperty(const BehaviourInstance& instance, const BehaviourField& field);
		bool SetFloatProperty(const BehaviourInstance& instance, const BehaviourField& field, float value);

		bool IsInt(const BehaviourField& field);
		int GetIntProperty(const BehaviourInstance& instance, const BehaviourField& field);
		bool SetIntProperty(const BehaviourInstance& instance, const BehaviourField& field, int value);

		int GetStringTypeID();
		bool IsString(const BehaviourField& field);
		std::string GetStringProperty(const BehaviourInstance& instance, const BehaviourField& field);
		bool SetStringProperty(const BehaviourInstance& instance, const BehaviourField& field, std::string value);

		inline bool CompiliationSuccess() const { return m_compilationSuccess; }
	};
}
