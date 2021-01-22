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
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include "math/mat4.h"
#include "entity/entity.h"
#include "entity/components/transform.h"

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

	/* Register Crimson::vec2 */
	static void RegisterVec2(asIScriptEngine* engine) {
		/* vec2 constructor wrappers */
		struct X {
			static void construct(void* memory) {
			       new (memory) vec2();
			}
			
			static void constructS(float scalar, void* memory) {
				new(memory) vec2(scalar);
			}
					
			static void constructXY(float x, float y, void* memory) {
				new(memory) vec2(x, y);
			}
		};

		int r;

		r = engine->RegisterObjectType("vec2", sizeof(vec2), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CAK | asGetTypeTraits<vec2>()); assert(r >= 0);
		r = engine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(X::construct), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f(float)", asFUNCTION(X::constructS), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f(float, float)", asFUNCTION(X::constructXY), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectProperty("vec2", "float x", asOFFSET(vec2, x)); assert(r >= 0);
		r = engine->RegisterObjectProperty("vec2", "float y", asOFFSET(vec2, y)); assert(r >= 0);
		
		/* Methods */
		r = engine->RegisterObjectMethod("vec2", "float magnitude() const", asMETHOD(vec2, magnitude), asCALL_THISCALL);
		r = engine->RegisterObjectMethod("vec2", "vec2 normalised() const", asMETHOD(vec2, normalised), asCALL_THISCALL);
		r = engine->RegisterObjectMethod("vec2", "float distance(const vec2&in) const", asMETHOD(vec2, distance), asCALL_THISCALL);
		r = engine->RegisterObjectMethod("vec2", "float dot(const vec2&in) const", asMETHOD(vec2, dot), asCALL_THISCALL);
	}

	/* Register Crimson::vec3 */
	static void RegisterVec3(asIScriptEngine* engine) {
		/* vec2 constructor wrappers */
		struct X {
			static void construct(void* memory) {
			       new (memory) vec3();
			}
			
			static void constructS(float scalar, void* memory) {
				new(memory) vec3(scalar);
			}
					
			static void constructX(float x, float y, float z, void* memory) {
				new(memory) vec3(x, y, z);
			}
		};

		int r;

		r = engine->RegisterObjectType("vec3", sizeof(vec3), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CAK | asGetTypeTraits<vec2>()); assert(r >= 0);
		r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(X::construct), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(float)", asFUNCTION(X::constructS), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(float, float, float)", asFUNCTION(X::constructX), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectProperty("vec3", "float x", asOFFSET(vec3, x)); assert(r >= 0);
		r = engine->RegisterObjectProperty("vec3", "float y", asOFFSET(vec3, y)); assert(r >= 0);
		
		/* Methods */
		r = engine->RegisterObjectMethod("vec3", "float magnitude() const", asMETHOD(vec3, magnitude), asCALL_THISCALL);
		r = engine->RegisterObjectMethod("vec3", "vec3 normalised() const", asMETHOD(vec3, normalised), asCALL_THISCALL);
		r = engine->RegisterObjectMethod("vec3", "float distance(const vec3&in) const", asMETHOD(vec3, distance), asCALL_THISCALL);
		r = engine->RegisterObjectMethod("vec3", "float dot(const vec3&in) const", asMETHOD(vec3, dot), asCALL_THISCALL);
	}

	static void RegisterComponents(asIScriptEngine* engine) {
		struct X {

		};

		int r;

		/* Transform */
		r = engine->RegisterObjectType("Transform", 0, asOBJ_REF | asOBJ_NOCOUNT); assert(r >= 0);
		r = engine->RegisterObjectProperty("Transform", "vec3 translation", asOFFSET(TransformComponent, translation)); assert(r >= 0);
		r = engine->RegisterObjectProperty("Transform", "vec3 rotation", asOFFSET(TransformComponent, rotation)); assert(r >= 0);
		r = engine->RegisterObjectProperty("Transform", "vec3 scale", asOFFSET(TransformComponent, scale)); assert(r >= 0);
	}

	static void RegisterEntity(asIScriptEngine* e) {
		/* So that nested functions can also use the engine */
		static asIScriptEngine* engine = e;

		/* Wrappers */
		struct X {
			static void GetComponent(void *ref, int typeID, Entity* entity) {
				if (typeID == engine->GetTypeIdByDecl("Transform@")) {
					TransformComponent** t = (TransformComponent**)ref;
					(*t) = entity->GetComponent<TransformComponent>();
				} else {
					Log(LogType::ERROR, "%s is not a valid component type", engine->GetTypeDeclaration(typeID));
				}
			}
		};

		int r;
		r = engine->RegisterObjectType("Entity", 0, asOBJ_REF | asOBJ_NOCOUNT); assert(r >= 0);
		r = engine->RegisterObjectMethod("Entity", "void GetComponent(?&out)", asFUNCTION(X::GetComponent), asCALL_CDECL_OBJLAST);
		r = engine->RegisterObjectMethod("Entity", "void Destroy()", asMETHOD(Entity, Destroy), asCALL_THISCALL);
	}

	/* Register Crimson::vec4 */
	static void RegisterVec4(asIScriptEngine* engine) {
		/* vec2 constructor wrappers */
		struct X {
			static void construct(void* memory) {
			       new (memory) vec4();
			}
			
			static void constructS(float scalar, void* memory) {
				new(memory) vec4(scalar);
			}
					
			static void constructX(float x, float y, float z, float w, void* memory) {
				new(memory) vec4(x, y, z, w);
			}
		};

		int r;

		r = engine->RegisterObjectType("vec4", sizeof(vec4), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CAK | asGetTypeTraits<vec2>()); assert(r >= 0);
		r = engine->RegisterObjectBehaviour("vec4", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(X::construct), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectBehaviour("vec4", asBEHAVE_CONSTRUCT, "void f(float)", asFUNCTION(X::constructS), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectBehaviour("vec4", asBEHAVE_CONSTRUCT, "void f(float, float, float, float)", asFUNCTION(X::constructX), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectProperty("vec4", "float x", asOFFSET(vec4, x)); assert(r >= 0);
		r = engine->RegisterObjectProperty("vec4", "float y", asOFFSET(vec4, y)); assert(r >= 0);
		
		/* Methods */
		r = engine->RegisterObjectMethod("vec4", "float dot(const vec4&in) const", asMETHOD(vec4, dot), asCALL_THISCALL); assert(r >= 0);
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

		/* Maths */
		RegisterVec2(engine);
		RegisterVec3(engine);
		RegisterVec4(engine);

		/* Entity */
		RegisterComponents(engine);
		RegisterEntity(engine);
	}
}
