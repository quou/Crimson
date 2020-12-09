#pragma once

#include <string>
#include <vector>

class asIScriptEngine;
class asIScriptContext;
class asIScriptModule;
class asIScriptObject;
class asITypeInfo;

#include "AssetManager.h"

namespace Crimson {
	struct LinterMessage {
		int line;
		int col;
		std::string message;
	};

	class Linter {
	private:
		std::string m_code;
		std::vector<LinterMessage> m_messages;
		std::vector<std::string> m_classes;

		asIScriptEngine* m_asEngine;
		asIScriptModule* m_asModule;
		asITypeInfo* m_baseTypeInfo;
	public:
		Linter();
		~Linter();

		inline void AddMessage(const LinterMessage& linterMessage) {
			m_messages.push_back(linterMessage);
		}

		std::vector<LinterMessage>& Lint(const std::string& code);
		std::vector<LinterMessage>& LintDir(const std::string& dir);

		inline std::vector<std::string>& GetClassNames() {return m_classes;}
	};
}
