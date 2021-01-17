#pragma once

#include <vector>
#include <string>

class asIScriptEngine;

namespace Crimson {
	class ScriptCompiler {
	private:
		asIScriptEngine* m_engine;

		std::vector<std::string> m_code;
	public:
		ScriptCompiler();
		virtual ~ScriptCompiler();

		void AddCode(const char* path);
		
		void Compile(const char* moduleName, const char* outputPath);
	};
}