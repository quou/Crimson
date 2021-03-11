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
#include "scriptkeys.h"
#include "logger.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include "math/mat4.h"
#include "entity/entity.h"
#include "entity/components/transform.h"
#include "entity/components/lights.h"
#include "entity/components/renderable.h"
#include "input/input.h"

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
		r = engine->RegisterObjectProperty("vec3", "float z", asOFFSET(vec3, z)); assert(r >= 0);
		
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

		/* Point Light */
		r = engine->RegisterObjectType("PointLight", 0, asOBJ_REF | asOBJ_NOCOUNT); assert(r >= 0);
		r = engine->RegisterObjectProperty("PointLight", "vec3 color", asOFFSET(PointLightComponent, color)); assert(r >= 0);
		r = engine->RegisterObjectProperty("PointLight", "float intensity", asOFFSET(PointLightComponent, intensity)); assert(r >= 0);
		r = engine->RegisterObjectProperty("PointLight", "float constant", asOFFSET(PointLightComponent, constant)); assert(r >= 0);
		r = engine->RegisterObjectProperty("PointLight", "float linear", asOFFSET(PointLightComponent, linear)); assert(r >= 0);
		r = engine->RegisterObjectProperty("PointLight", "float quadratic", asOFFSET(PointLightComponent, quadratic)); assert(r >= 0);

		/* Sky Light */
		r = engine->RegisterObjectType("SkyLight", 0, asOBJ_REF | asOBJ_NOCOUNT); assert(r >= 0);
		r = engine->RegisterObjectProperty("SkyLight", "vec3 color", asOFFSET(SkyLightComponent, color)); assert(r >= 0);
		r = engine->RegisterObjectProperty("SkyLight", "float intensity", asOFFSET(SkyLightComponent, intensity)); assert(r >= 0);

		/* Sun */
		r = engine->RegisterObjectType("Sun", 0, asOBJ_REF | asOBJ_NOCOUNT); assert(r >= 0);
		r = engine->RegisterObjectProperty("Sun", "vec3 direction", asOFFSET(SunComponent, direction)); assert(r >= 0);
		r = engine->RegisterObjectProperty("Sun", "vec3 color", asOFFSET(SunComponent, color)); assert(r >= 0);
		r = engine->RegisterObjectProperty("Sun", "float intensity", asOFFSET(SunComponent, intensity)); assert(r >= 0);

		/* Camera */
		r = engine->RegisterObjectType("Camera", 0, asOBJ_REF | asOBJ_NOCOUNT); assert(r >= 0);
		r = engine->RegisterObjectProperty("Camera", "bool active", asOFFSET(CameraComponent, active)); assert(r >= 0);
	}

	asIScriptEngine* g_engine;

	static void RegisterEntity(asIScriptEngine* engine) {
		/* So that nested functions can also use the engine */
		g_engine = engine;

		/* Wrappers */
		struct X {
			static void GetComponent(void *ref, int typeID, Entity* entity) {
				if (typeID == g_engine->GetTypeIdByDecl("Transform@")) { /* Transform */
					TransformComponent** t = (TransformComponent**)ref;
					(*t) = entity->GetComponent<TransformComponent>();
				
				} else if (typeID == g_engine->GetTypeIdByDecl("PointLight@")) /* Point Light */ {
					PointLightComponent** t = (PointLightComponent**)ref;
					(*t) = entity->GetComponent<PointLightComponent>();

				} else if (typeID == g_engine->GetTypeIdByDecl("SkyLight@")) /* Sky Light */ {
					SkyLightComponent** t = (SkyLightComponent**)ref;
					(*t) = entity->GetComponent<SkyLightComponent>();

				} else if (typeID == g_engine->GetTypeIdByDecl("Sun@")) /* Sun */ {
					SunComponent** t = (SunComponent**)ref;
					(*t) = entity->GetComponent<SunComponent>();

				} else if (typeID == g_engine->GetTypeIdByDecl("Camera@")) /* Camera */ {
					CameraComponent** t = (CameraComponent**)ref;
					(*t) = entity->GetComponent<CameraComponent>();
				} else {
					Log(LogType::ERROR, "%s is not a valid component type", g_engine->GetTypeDeclaration(typeID));
				}
			}
		};

		int r;
		r = engine->RegisterObjectType("Entity", 0, asOBJ_REF | asOBJ_NOCOUNT); assert(r >= 0);
		r = engine->RegisterObjectMethod("Entity", "void GetComponent(?&out)", asFUNCTION(X::GetComponent), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectMethod("Entity", "void Destroy()", asMETHOD(Entity, Destroy), asCALL_THISCALL); assert(r >= 0);
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
		r = engine->RegisterObjectProperty("vec4", "float z", asOFFSET(vec4, z)); assert(r >= 0);
		r = engine->RegisterObjectProperty("vec4", "float w", asOFFSET(vec4, w)); assert(r >= 0);
		
		/* Methods */
		r = engine->RegisterObjectMethod("vec4", "float dot(const vec4&in) const", asMETHOD(vec4, dot), asCALL_THISCALL); assert(r >= 0);
	}

	static void RegisterInput(asIScriptEngine* engine) {
		int r;

		r = engine->RegisterGlobalProperty("const int KEY_UNKNOWN", &SCRIPT_KEY_UNKNOWN); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_SPACE", &SCRIPT_KEY_SPACE); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_APOSTROPHE", &SCRIPT_KEY_APOSTROPHE); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_COMMA", &SCRIPT_KEY_COMMA); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_MINUS", &SCRIPT_KEY_MINUS); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_PERIOD", &SCRIPT_KEY_PERIOD); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_SLASH", &SCRIPT_KEY_SLASH); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_0", &SCRIPT_KEY_0); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_1", &SCRIPT_KEY_1); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_2", &SCRIPT_KEY_2); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_3", &SCRIPT_KEY_3); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_4", &SCRIPT_KEY_4); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_5", &SCRIPT_KEY_5); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_6", &SCRIPT_KEY_6); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_7", &SCRIPT_KEY_7); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_8", &SCRIPT_KEY_8); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_9", &SCRIPT_KEY_9); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_SEMICOLON", &SCRIPT_KEY_SEMICOLON); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_EQUAL", &SCRIPT_KEY_EQUAL); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_A", &SCRIPT_KEY_A); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_B", &SCRIPT_KEY_B); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_C", &SCRIPT_KEY_C); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_D", &SCRIPT_KEY_D); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_E", &SCRIPT_KEY_E); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_F", &SCRIPT_KEY_F); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_G", &SCRIPT_KEY_G); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_H", &SCRIPT_KEY_H); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_J", &SCRIPT_KEY_J); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_K", &SCRIPT_KEY_K); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_L", &SCRIPT_KEY_L); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_M", &SCRIPT_KEY_M); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_O", &SCRIPT_KEY_O); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_P", &SCRIPT_KEY_P); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_Q", &SCRIPT_KEY_Q); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_R", &SCRIPT_KEY_R); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_S", &SCRIPT_KEY_S); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_T", &SCRIPT_KEY_T); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_U", &SCRIPT_KEY_U); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_V", &SCRIPT_KEY_V); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_W", &SCRIPT_KEY_W); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_X", &SCRIPT_KEY_X); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_Y", &SCRIPT_KEY_Y); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_Z", &SCRIPT_KEY_Z); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_LEFT_BRACKET", &SCRIPT_KEY_LEFT_BRACKET); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_BACKSLASH", &SCRIPT_KEY_BACKSLASH); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_RIGHT_BRACKET", &SCRIPT_KEY_RIGHT_BRACKET); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_GRAVE_ACCENT", &SCRIPT_KEY_GRAVE_ACCENT); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_WORLD_1", &SCRIPT_KEY_WORLD_1); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_WORLD_2", &SCRIPT_KEY_WORLD_2); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_ESCAPE", &SCRIPT_KEY_ESCAPE); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_ENTER", &SCRIPT_KEY_ENTER); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_TAB", &SCRIPT_KEY_TAB); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_BACKSPACE", &SCRIPT_KEY_BACKSPACE); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_INSERT", &SCRIPT_KEY_INSERT); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_DELETE", &SCRIPT_KEY_DELETE); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_RIGHT", &SCRIPT_KEY_RIGHT); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_LEFT", &SCRIPT_KEY_LEFT); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_DOWN", &SCRIPT_KEY_DOWN); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_UP", &SCRIPT_KEY_UP); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_PAGE_UP", &SCRIPT_KEY_PAGE_UP); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_PAGE_DOWN", &SCRIPT_KEY_PAGE_DOWN); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_HOME", &SCRIPT_KEY_HOME); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_END", &SCRIPT_KEY_END); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_CAPS_LOCK", &SCRIPT_KEY_CAPS_LOCK); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_SCROLL_LOCK", &SCRIPT_KEY_SCROLL_LOCK); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_NUM_LOCK", &SCRIPT_KEY_NUM_LOCK); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_PRINT_SCREEN", &SCRIPT_KEY_PRINT_SCREEN); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_PAUSE", &SCRIPT_KEY_PAUSE); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_F1", &SCRIPT_KEY_F1); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_F2", &SCRIPT_KEY_F2); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_F3", &SCRIPT_KEY_F3); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_F4", &SCRIPT_KEY_F4); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_F5", &SCRIPT_KEY_F5); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_F6", &SCRIPT_KEY_F6); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_F7", &SCRIPT_KEY_F7); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_F8", &SCRIPT_KEY_F8); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_F9", &SCRIPT_KEY_F9); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_F10", &SCRIPT_KEY_F10); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_F11", &SCRIPT_KEY_F11); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_F12", &SCRIPT_KEY_F12); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_F13", &SCRIPT_KEY_F13); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_F14", &SCRIPT_KEY_F14); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_F15", &SCRIPT_KEY_F15); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_F16", &SCRIPT_KEY_F16); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_F17", &SCRIPT_KEY_F17); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_F18", &SCRIPT_KEY_F18); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_F19", &SCRIPT_KEY_F19); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_F20", &SCRIPT_KEY_F20); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_F21", &SCRIPT_KEY_F21); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_F22", &SCRIPT_KEY_F22); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_F23", &SCRIPT_KEY_F23); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_F24", &SCRIPT_KEY_F24); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_F25", &SCRIPT_KEY_F25); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_KP_0", &SCRIPT_KEY_KP_0); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_KP_1", &SCRIPT_KEY_KP_1); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_KP_2", &SCRIPT_KEY_KP_2); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_KP_3", &SCRIPT_KEY_KP_3); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_KP_4", &SCRIPT_KEY_KP_4); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_KP_5", &SCRIPT_KEY_KP_5); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_KP_6", &SCRIPT_KEY_KP_6); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_KP_7", &SCRIPT_KEY_KP_7); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_KP_8", &SCRIPT_KEY_KP_8); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_KP_9", &SCRIPT_KEY_KP_9); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_KP_DECIMAL", &SCRIPT_KEY_KP_DECIMAL); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_KP_DIVIDE", &SCRIPT_KEY_KP_DIVIDE); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_KP_MULTIPLY", &SCRIPT_KEY_KP_MULTIPLY); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_KP_SUBTRACT", &SCRIPT_KEY_KP_SUBTRACT); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_KP_ADD", &SCRIPT_KEY_KP_ADD); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_KP_ENTER", &SCRIPT_KEY_KP_ENTER); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_KP_EQUAL", &SCRIPT_KEY_KP_EQUAL); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_LEFT_SHIFT", &SCRIPT_KEY_LEFT_SHIFT); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_LEFT_CONTROL", &SCRIPT_KEY_LEFT_CONTROL); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_LEFT_ALT", &SCRIPT_KEY_LEFT_ALT); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_LEFT_SUPER", &SCRIPT_KEY_LEFT_SUPER); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_RIGHT_SHIFT", &SCRIPT_KEY_RIGHT_SHIFT); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_RIGHT_CONTROL", &SCRIPT_KEY_RIGHT_CONTROL); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_RIGHT_ALT", &SCRIPT_KEY_RIGHT_ALT); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_RIGHT_SUPER", &SCRIPT_KEY_RIGHT_SUPER); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_MENU", &SCRIPT_KEY_MENU); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int KEY_COUNT", &SCRIPT_KEY_COUNT); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int MOD_SHIFT", &SCRIPT_MOD_SHIFT); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int MOD_CONTROL", &SCRIPT_MOD_CONTROL); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int MOD_ALT", &SCRIPT_MOD_ALT); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int MOD_SUPER", &SCRIPT_MOD_SUPER); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int MOD_CAPS_LOCK", &SCRIPT_MOD_CAPS_LOCK); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int MOD_NUM_LOCK", &SCRIPT_MOD_NUM_LOCK); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int MOUSE_BUTTON_1", &SCRIPT_MOUSE_BUTTON_1); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int MOUSE_BUTTON_2", &SCRIPT_MOUSE_BUTTON_2); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int MOUSE_BUTTON_3", &SCRIPT_MOUSE_BUTTON_3); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int MOUSE_BUTTON_4", &SCRIPT_MOUSE_BUTTON_4); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int MOUSE_BUTTON_5", &SCRIPT_MOUSE_BUTTON_5); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int MOUSE_BUTTON_6", &SCRIPT_MOUSE_BUTTON_6); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int MOUSE_BUTTON_7", &SCRIPT_MOUSE_BUTTON_7); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int MOUSE_BUTTON_8", &SCRIPT_MOUSE_BUTTON_8); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int MOUSE_BUTTON_COUNT", &SCRIPT_MOUSE_BUTTON_COUNT); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int MOUSE_BUTTON_LEFT", &SCRIPT_MOUSE_BUTTON_LEFT); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int MOUSE_BUTTON_RIGHT", &SCRIPT_MOUSE_BUTTON_RIGHT); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int MOUSE_BUTTON_MIDDLE", &SCRIPT_MOUSE_BUTTON_MIDDLE); assert(r >= 0);

		engine->SetDefaultNamespace("EventSystem");

		r = engine->RegisterGlobalFunction("bool KeyPressed(int key)", asFUNCTION(EventSystem::KeyPressed), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("bool KeyJustPressed(int key)", asFUNCTION(EventSystem::KeyJustPressed), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("bool KeyJustReleased(int key)", asFUNCTION(EventSystem::KeyJustReleased), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("bool MouseButtonPressed(int key)", asFUNCTION(EventSystem::MouseButtonPressed), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("bool MouseButtonJustPressed(int key)", asFUNCTION(EventSystem::MouseButtonJustPressed), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("bool MouseButtonJustReleased(int key)", asFUNCTION(EventSystem::MouseButtonJustReleased), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("vec2 GetMousePosition()", asFUNCTION(EventSystem::GetMousePosition), asCALL_CDECL); assert(r >= 0);

		engine->SetDefaultNamespace("");
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

		/* Base engine utils */
		RegisterInput(engine);

		/* Entity */
		RegisterComponents(engine);
		RegisterEntity(engine);
	}
}
