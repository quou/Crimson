#include <assert.h>

#include <angelscript/angelscript.h>
#include <scriptstdstring/scriptstdstring.h>

#include "scriptregister.h"

namespace Crimson {
	static void print(std::string& in) {
		printf("%s", in.c_str());
	}

	void ScriptRegister::Register(asIScriptEngine* engine) {
		int r;
		RegisterStdString(engine);
		r = engine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(print), asCALL_CDECL); assert(r >= 0);
	}
}