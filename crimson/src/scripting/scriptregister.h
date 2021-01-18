#pragma once

class asIScriptEngine;

namespace Crimson {
	class ScriptRegister {
	public:
		static void Register(asIScriptEngine* engine);
	};
}