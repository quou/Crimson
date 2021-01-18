#include <assert.h>

#include <angelscript/angelscript.h>
#include <scriptstdstring/scriptstdstring.h>
#include <scriptmath/scriptmath.h>
#include <scriptmath/scriptmathcomplex.h>
#include <scriptarray/scriptarray.h>
#include <scriptbuilder/scriptbuilder.h>
#include <scriptdictionary/scriptdictionary.h>
#include <scriptfile/scriptfile.h>
#include <datetime/datetime.h>
#include <scripthelper/scripthelper.h>

#include "scriptregister.h"
#include "logger.h"

namespace Crimson {
	void MessageCallback(const asSMessageInfo *msg, void *param) {
		LogType type = LogType::ERROR;

		if (msg->type == asMSGTYPE_WARNING) {
			type = LogType::WARNING;
		} else if (msg->type == asMSGTYPE_INFORMATION) {
			type = LogType::INFO;
		}

		Log(type, "%s (%d, %d) : %s", msg->section, msg->row, msg->col, msg->message);
	}

	static void ScriptLogInfo(std::string& message) {
		Log(LogType::INFO, message.c_str());
	}

	static void ScriptLogError(std::string& message) {
		Log(LogType::ERROR, message.c_str());
	}

	static void ScriptLogWarning(std::string& message) {
		Log(LogType::WARNING, message.c_str());
	}

	static std::string ScriptToString(double in) {
		char str[1024];
		sprintf(str, "%g", in);

		return str;
	}

	void ScriptRegister::Register(asIScriptEngine* engine) {
		engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
		
		int r;

		/* AngelScript standard library  */
		RegisterStdString(engine);
		RegisterScriptMath(engine);
		RegisterScriptMathComplex(engine);
		RegisterScriptArray(engine, true);
		RegisterScriptFile(engine);
		RegisterScriptDictionary(engine);
		RegisterScriptDateTime(engine);
		RegisterExceptionRoutines(engine);
		RegisterStdStringUtils(engine);

		/* I/O */
		r = engine->RegisterGlobalFunction("void print(const string&in)", asFUNCTION(ScriptLogInfo), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("void Log(const string&in)", asFUNCTION(ScriptLogInfo), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("void LogError(const string&in)", asFUNCTION(ScriptLogError), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("void LogWarning(const string&in)", asFUNCTION(ScriptLogWarning), asCALL_CDECL); assert(r >= 0);

		/* Utility functions */	
		r = engine->RegisterGlobalFunction("string ToString(double)", asFUNCTION(ScriptToString), asCALL_CDECL); assert(r >= 0);

	}
}
