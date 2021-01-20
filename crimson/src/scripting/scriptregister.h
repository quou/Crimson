#pragma once

#include "core.h"

class asIScriptEngine;

namespace Crimson {
	class CR_API ScriptRegister {
	public:
		static void Register(asIScriptEngine* engine);
	};
}
