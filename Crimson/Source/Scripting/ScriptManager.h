#pragma once

#include <vector>
#include <string>

class asIScriptEngine;

namespace Crimson {
	class ScriptManager {
	private:
		asIScriptEngine* m_asEngine;

		std::vector<std::string> m_codeFiles;
	public:
		ScriptManager();
		~ScriptManager();

		void AddScript(const std::string& code);
		void Flush();

		void Compile();
		void Init();
		void Update(float delta);
	};
}
