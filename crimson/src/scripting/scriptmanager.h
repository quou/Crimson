#pragma once

#include <vector>
#include <string>

#include "core.h"

class asIScriptEngine;
class asIScriptContext;
class asIScriptModule;
class asIScriptObject;
class asITypeInfo;

namespace Crimson {
	class CR_API ScriptManager {
	private:
		asIScriptEngine* m_asEngine;
		asIScriptContext* m_asContext;
		asIScriptModule* m_asModule;

		std::vector<std::string> m_codeFiles;
		
		bool m_compilationSuccess = false;
	public:
		ScriptManager();
		virtual ~ScriptManager();

		void AddScript(const std::string& code);

		void Compile(const char* moduleName);

		void CallFunc(const char* decl);

		void Flush();
	};
}