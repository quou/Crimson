#include "Register.h"

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

#include <glm/glm.hpp>

#include "Logger.h"
#include "AssetManager.h"
#include "SceneManagement/Components.h"
#include "SceneManagement/Entity.h"
#include "SceneManagement/Scene.h"

#include "Input.h"
#include "ScriptKeys.h"

namespace Crimson {
	static void scriptPrint(std::string &msg)	{
		CR_LOG("%s", msg.c_str());
	}

	static void scriptLogWarning(std::string &msg)	{
		CR_LOG_WARNING("%s", msg.c_str());
	}
	static void scriptLogError(std::string &msg)	{
		CR_LOG_ERROR("%s", msg.c_str());
	}
	static void scriptLogFatalError(std::string &msg)	{
		CR_LOG_FATAL_ERROR("%s", msg.c_str());
	}

	static std::string scriptToString(double val) {
		return std::to_string(val);
	}

	static std::string scriptToString(bool val) {
		if (val) {
			return "true";
		} else {
			return "false";
		}
	}

	static std::string scriptToString(glm::vec2 val) {
		return std::to_string(val.x) + ", " + std::to_string(val.y);
	}

	static std::string scriptToString(glm::vec3 val) {
		return std::to_string(val.x) + ", " + std::to_string(val.y) + ", " + std::to_string(val.z);
	}

	static std::string scriptToString(glm::vec4 val) {
		return std::to_string(val.x) + ", " + std::to_string(val.y) + ", " + std::to_string(val.z) + ", " + std::to_string(val.w);
	}

	void vec2_Constructor(void *memory) {
     new(memory) glm::vec2();
   }

   void vec2_ConstructorXY(void *memory, float xy) {
     new(memory) glm::vec2(xy);
   }

   void vec2_ConstructorX_Y(void *memory, float x, float y) {
     new(memory) glm::vec2(x,y);
   }

   void vec3_Constructor(void *memory) {
     new(memory) glm::vec3();
   }

   void vec3_ConstructorXYZ(void *memory, float xy) {
     new(memory) glm::vec3(xy);
   }

   void vec3_ConstructorX_Y_Z(void *memory, float x, float y, float z) {
     new(memory) glm::vec3(x,y,z);
   }

   void vec4_Constructor(void *memory) {
     new(memory) glm::vec4();
   }

   void vec4_ConstructorXYZW(void *memory, float xy) {
     new(memory) glm::vec4(xy);
   }

   void vec4_ConstructorX_Y_Z_W(void *memory, float x, float y, float z, float w) {
     new(memory) glm::vec4(x,y,z,w);
   }

	void quat_Constructor(void *memory) {
     new(memory) glm::quat();
   }
	void quat_ConstructorVEC(void *memory, const glm::vec3& v) {
     new(memory) glm::quat(v);
   }

	void quat_Constructor_XYZW(void *memory, float w, float x, float y, float z) {
     new(memory) glm::quat(w, x, y, z);
   }

	void Rigidbody_Constructor(void *memory) {
		new(memory) Rigidbody();
	}

	void Rigidbody_Destructor(void *memory) {
		((Rigidbody*)memory)->~Rigidbody();
	}

	static TransformComponent& Entity_GetTransformComponent(Entity* ent) {
		return ent->GetComponent<TransformComponent>();
	}

	static ScriptComponent& Entity_GetScriptComponent(Entity* ent) {
		return ent->GetComponent<ScriptComponent>();
	}

	static PhysicsComponent& Entity_GetPhysicsComponent(Entity* ent) {
		return ent->GetComponent<PhysicsComponent>();
	}

	static bool Entity_HasTransformComponent(Entity* ent) {
		return ent->HasComponent<TransformComponent>();
	}

	static MeshFilterComponent& Entity_GetMeshFilterComponent(Entity* ent) {
		return ent->GetComponent<MeshFilterComponent>();
	}

	static bool Entity_HasMeshFilterComponent(Entity* ent) {
		return ent->HasComponent<MeshFilterComponent>();
	}

	static MaterialComponent& Entity_GetMaterialComponent(Entity* ent) {
		return ent->GetComponent<MaterialComponent>();
	}

	static bool Entity_HasMaterialComponent(Entity* ent) {
		return ent->HasComponent<MaterialComponent>();
	}

	static bool Entity_HasScriptComponent(Entity* ent) {
		return ent->HasComponent<ScriptComponent>();
	}

	static bool Entity_HasPhysicsComponent(Entity* ent) {
		return ent->HasComponent<PhysicsComponent>();
	}

	glm::vec3 Script_EulerAngles(const glm::quat& q) {
		return glm::eulerAngles(q);
	}

   template <typename T>
   void RegisterVector(asIScriptEngine* engine, const std::string& asTypename) {
      const char* asTypenamePtr = asTypename.c_str();

      int r;

      r = engine->RegisterObjectType(asTypenamePtr, sizeof(T), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<T>()); assert( r >= 0 );
      r = engine->RegisterObjectMethod(asTypenamePtr, std::string(asTypename + "& opAddAssign(const " + asTypename + " &in) const").c_str(), asMETHODPR(T, operator+=, (const T&), T&), asCALL_THISCALL); assert( r >= 0 );
      r = engine->RegisterObjectMethod(asTypenamePtr, std::string(asTypename + "& opSubAssign(const " + asTypename + " &in) const").c_str(), asMETHODPR(T, operator-=, (const T&), T&), asCALL_THISCALL); assert( r >= 0 );
      r = engine->RegisterObjectMethod(asTypenamePtr, std::string(asTypename + "& opMulAssign(const " + asTypename + " &in) const").c_str(), asMETHODPR(T, operator*=, (const T&), T&), asCALL_THISCALL); assert( r >= 0 );
      r = engine->RegisterObjectMethod(asTypenamePtr, std::string(asTypename + "& opDivAssign(const " + asTypename + " &in) const").c_str(), asMETHODPR(T, operator/=, (const T&), T&), asCALL_THISCALL); assert( r >= 0 );
      r = engine->RegisterObjectMethod(asTypenamePtr, std::string(asTypename + "& opAssign(const " + asTypename + " &in) const").c_str(), asMETHODPR(T, operator=, (const T&), T&), asCALL_THISCALL); assert( r >= 0 );
      r = engine->RegisterObjectMethod(asTypenamePtr, std::string(asTypename + "& opAdd(const " + asTypename + "& in) const").c_str(), asFUNCTIONPR(glm::operator+, (const T&, const T&), T), asCALL_CDECL_OBJLAST); assert( r >= 0 );
      r = engine->RegisterObjectMethod(asTypenamePtr, std::string(asTypename + "& opSub(const " + asTypename + "& in) const").c_str(), asFUNCTIONPR(glm::operator-, (const T&, const T&), T), asCALL_CDECL_OBJLAST); assert( r >= 0 );
      r = engine->RegisterObjectMethod(asTypenamePtr, std::string(asTypename + "& opMul(const " + asTypename + "& in) const").c_str(), asFUNCTIONPR(glm::operator*, (const T&, const T&), T), asCALL_CDECL_OBJLAST); assert( r >= 0 );
      r = engine->RegisterObjectMethod(asTypenamePtr, std::string(asTypename + "& opDiv(const " + asTypename + "& in) const").c_str(), asFUNCTIONPR(glm::operator/, (const T&, const T&), T), asCALL_CDECL_OBJLAST); assert( r >= 0 );
   }

	void RegisterScript(asIScriptEngine* engine) {
		RegisterStdString(engine);
		RegisterScriptMath(engine);
		RegisterScriptMathComplex(engine);
		RegisterScriptArray(engine, true);
		RegisterScriptFile(engine);
		RegisterScriptDictionary(engine);
		RegisterScriptDateTime(engine);
		RegisterExceptionRoutines(engine);

		int r;

		r = engine->RegisterGlobalFunction("void print(const string& in)", asFUNCTION(scriptPrint), asCALL_CDECL); assert(r >= 0);

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
		r = engine->RegisterGlobalProperty("const int KEY_LAST", &SCRIPT_KEY_LAST); assert(r >= 0);
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
		r = engine->RegisterGlobalProperty("const int MOUSE_BUTTON_LAST", &SCRIPT_MOUSE_BUTTON_LAST); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int MOUSE_BUTTON_LEFT", &SCRIPT_MOUSE_BUTTON_LEFT); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int MOUSE_BUTTON_RIGHT", &SCRIPT_MOUSE_BUTTON_RIGHT); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int MOUSE_BUTTON_MIDDLE", &SCRIPT_MOUSE_BUTTON_MIDDLE); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_JOYSTICK_1", &SCRIPT_JOYSTICK_1); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_JOYSTICK_2", &SCRIPT_JOYSTICK_2); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_JOYSTICK_3", &SCRIPT_JOYSTICK_3); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_JOYSTICK_4", &SCRIPT_JOYSTICK_4); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_JOYSTICK_5", &SCRIPT_JOYSTICK_5); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_JOYSTICK_6", &SCRIPT_JOYSTICK_6); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_JOYSTICK_7", &SCRIPT_JOYSTICK_7); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_JOYSTICK_8", &SCRIPT_JOYSTICK_8); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_JOYSTICK_9", &SCRIPT_JOYSTICK_9); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_JOYSTICK_10", &SCRIPT_JOYSTICK_10); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_JOYSTICK_11", &SCRIPT_JOYSTICK_11); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_JOYSTICK_12", &SCRIPT_JOYSTICK_12); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_JOYSTICK_13", &SCRIPT_JOYSTICK_13); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_JOYSTICK_14", &SCRIPT_JOYSTICK_14); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_JOYSTICK_15", &SCRIPT_JOYSTICK_15); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_JOYSTICK_16", &SCRIPT_JOYSTICK_16); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_BUTTON_A", &SCRIPT_GAMEPAD_BUTTON_A); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_BUTTON_B", &SCRIPT_GAMEPAD_BUTTON_B); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_BUTTON_X", &SCRIPT_GAMEPAD_BUTTON_X); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_BUTTON_Y", &SCRIPT_GAMEPAD_BUTTON_Y); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_BUTTON_LEFT_BUMPER", &SCRIPT_GAMEPAD_BUTTON_LEFT_BUMPER); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_BUTTON_RIGHT_BUMPER", &SCRIPT_GAMEPAD_BUTTON_RIGHT_BUMPER); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_BUTTON_BACK", &SCRIPT_GAMEPAD_BUTTON_BACK); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_BUTTON_START", &SCRIPT_GAMEPAD_BUTTON_START); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_BUTTON_GUIDE", &SCRIPT_GAMEPAD_BUTTON_GUIDE); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_BUTTON_LEFT_THUMB", &SCRIPT_GAMEPAD_BUTTON_LEFT_THUMB); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_BUTTON_RIGHT_THUMB", &SCRIPT_GAMEPAD_BUTTON_RIGHT_THUMB); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_BUTTON_DPAD_UP", &SCRIPT_GAMEPAD_BUTTON_DPAD_UP); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_BUTTON_DPAD_RIGHT", &SCRIPT_GAMEPAD_BUTTON_DPAD_RIGHT); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_BUTTON_DPAD_DOWN", &SCRIPT_GAMEPAD_BUTTON_DPAD_DOWN); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_BUTTON_DPAD_LEFT", &SCRIPT_GAMEPAD_BUTTON_DPAD_LEFT); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_BUTTON_LAST", &SCRIPT_GAMEPAD_BUTTON_LAST); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_BUTTON_CROSS", &SCRIPT_GAMEPAD_BUTTON_CROSS); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_BUTTON_CIRCLE", &SCRIPT_GAMEPAD_BUTTON_CIRCLE); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_BUTTON_SQUARE", &SCRIPT_GAMEPAD_BUTTON_SQUARE); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_BUTTON_TRIANGLE", &SCRIPT_GAMEPAD_BUTTON_TRIANGLE); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_AXIS_LEFT_X", &SCRIPT_GAMEPAD_AXIS_LEFT_X); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_AXIS_LEFT_Y", &SCRIPT_GAMEPAD_AXIS_LEFT_Y); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_AXIS_RIGHT_X", &SCRIPT_GAMEPAD_AXIS_RIGHT_X); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_AXIS_RIGHT_Y", &SCRIPT_GAMEPAD_AXIS_RIGHT_Y); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_AXIS_LEFT_TRIGGER", &SCRIPT_GAMEPAD_AXIS_LEFT_TRIGGER); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_AXIS_RIGHT_TRIGGER", &SCRIPT_GAMEPAD_AXIS_RIGHT_TRIGGER); assert(r >= 0);
		r = engine->RegisterGlobalProperty("const int SCRIPT_GAMEPAD_AXIS_LAST", &SCRIPT_GAMEPAD_AXIS_LAST); assert(r >= 0);


		RegisterVector<glm::vec2>(engine, "vec2");
      r = engine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(vec2_Constructor), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
      r = engine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f(float)", asFUNCTION(vec2_ConstructorXY), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
      r = engine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f(float, float)", asFUNCTION(vec2_ConstructorX_Y), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
      r = engine->RegisterObjectProperty("vec2", "float x", asOFFSET(glm::vec2,x)); assert( r >= 0 );
      r = engine->RegisterObjectProperty("vec2", "float y", asOFFSET(glm::vec2,y)); assert( r >= 0 );

      RegisterVector<glm::vec3>(engine, "vec3");
      r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(vec3_Constructor), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
      r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(float)", asFUNCTION(vec3_ConstructorXYZ), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
      r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(float, float, float)", asFUNCTION(vec3_ConstructorX_Y_Z), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
      r = engine->RegisterObjectProperty("vec3", "float x", asOFFSET(glm::vec3,x)); assert( r >= 0 );
      r = engine->RegisterObjectProperty("vec3", "float y", asOFFSET(glm::vec3,y)); assert( r >= 0 );
      r = engine->RegisterObjectProperty("vec3", "float z", asOFFSET(glm::vec3,z)); assert( r >= 0 );

      RegisterVector<glm::vec4>(engine, "vec4");
      r = engine->RegisterObjectBehaviour("vec4", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(vec4_Constructor), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
      r = engine->RegisterObjectBehaviour("vec4", asBEHAVE_CONSTRUCT, "void f(float)", asFUNCTION(vec4_ConstructorXYZW), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
      r = engine->RegisterObjectBehaviour("vec4", asBEHAVE_CONSTRUCT, "void f(float, float, float, float)", asFUNCTION(vec4_ConstructorX_Y_Z_W), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
      r = engine->RegisterObjectProperty("vec4", "float x", asOFFSET(glm::vec4,x)); assert( r >= 0 );
      r = engine->RegisterObjectProperty("vec4", "float y", asOFFSET(glm::vec4,y)); assert( r >= 0 );
      r = engine->RegisterObjectProperty("vec4", "float z", asOFFSET(glm::vec4,z)); assert( r >= 0 );
      r = engine->RegisterObjectProperty("vec4", "float w", asOFFSET(glm::vec4,w)); assert( r >= 0 );

		r = engine->RegisterObjectType("quat", sizeof(glm::quat), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<glm::quat>()); assert( r >= 0 );
      r = engine->RegisterObjectBehaviour("quat", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(quat_Constructor), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
      r = engine->RegisterObjectBehaviour("quat", asBEHAVE_CONSTRUCT, "void f(float, float, float, float)", asFUNCTION(quat_Constructor_XYZW), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
      r = engine->RegisterObjectBehaviour("quat", asBEHAVE_CONSTRUCT, "void f(const vec3 &in)", asFUNCTION(quat_ConstructorVEC), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
      r = engine->RegisterObjectProperty("quat", "float x", asOFFSET(glm::vec4,x)); assert( r >= 0 );
      r = engine->RegisterObjectProperty("quat", "float y", asOFFSET(glm::vec4,y)); assert( r >= 0 );
      r = engine->RegisterObjectProperty("quat", "float z", asOFFSET(glm::vec4,z)); assert( r >= 0 );
      r = engine->RegisterObjectProperty("quat", "float w", asOFFSET(glm::vec4,w)); assert( r >= 0 );

		r = engine->RegisterGlobalFunction("string to_string(double)", asFUNCTIONPR(scriptToString, (double), std::string), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("string to_string(bool)", asFUNCTIONPR(scriptToString, (bool), std::string), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("string to_string(vec2)", asFUNCTIONPR(scriptToString, (glm::vec2), std::string), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("string to_string(vec3)", asFUNCTIONPR(scriptToString, (glm::vec3), std::string), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("string to_string(vec4)", asFUNCTIONPR(scriptToString, (glm::vec4), std::string), asCALL_CDECL); assert(r >= 0);

		r = engine->RegisterGlobalFunction("vec2 toRadians(const vec2& in)", asFUNCTIONPR(glm::radians, (const glm::vec2&), glm::vec2), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("vec3 toRadians(const vec3& in)", asFUNCTIONPR(glm::radians, (const glm::vec3&), glm::vec3), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("vec4 toRadians(const vec4& in)", asFUNCTIONPR(glm::radians, (const glm::vec4&), glm::vec4), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("float toRadians(float)", asFUNCTIONPR(glm::radians, (float), float), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("vec2 toDegrees(const vec2& in)", asFUNCTIONPR(glm::degrees, (const glm::vec2&), glm::vec2), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("vec3 toDegrees(const vec3& in)", asFUNCTIONPR(glm::degrees, (const glm::vec3&), glm::vec3), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("vec4 toDegrees(const vec4& in)", asFUNCTIONPR(glm::degrees, (const glm::vec4&), glm::vec4), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("float toDegrees(float)", asFUNCTIONPR(glm::degrees, (float), float), asCALL_CDECL); assert(r >= 0);

		r = engine->RegisterGlobalFunction("vec3 eulerAngles(const quat& in)", asFUNCTIONPR(glm::eulerAngles, (const glm::quat&), glm::vec3), asCALL_CDECL); assert(r >= 0);

		r = engine->RegisterObjectType("TransformComponent", sizeof(TransformComponent), asOBJ_VALUE | asOBJ_POD); assert(r >= 0);
      r = engine->RegisterObjectProperty("TransformComponent", "vec3 position", asOFFSET(TransformComponent,position)); assert(r >= 0);
      r = engine->RegisterObjectProperty("TransformComponent", "vec3 scale", asOFFSET(TransformComponent,scale)); assert(r >= 0);
      r = engine->RegisterObjectProperty("TransformComponent", "quat rotation", asOFFSET(TransformComponent,rotation)); assert(r >= 0);
      r = engine->RegisterObjectProperty("TransformComponent", "string tag", asOFFSET(TransformComponent,tag)); assert(r >= 0);
      r = engine->RegisterObjectProperty("TransformComponent", "string name", asOFFSET(TransformComponent,name)); assert(r >= 0);

		r = engine->RegisterObjectType("ScriptComponent", sizeof(ScriptComponent), asOBJ_VALUE | asOBJ_POD); assert(r >= 0);

		r = engine->RegisterObjectType("Rigidbody", sizeof(Rigidbody), asOBJ_VALUE); assert(r >= 0);
		r = engine->RegisterObjectBehaviour("Rigidbody", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Rigidbody_Constructor), asCALL_CDECL_OBJFIRST); assert(r >= 0);
		r = engine->RegisterObjectBehaviour("Rigidbody", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(Rigidbody_Destructor), asCALL_CDECL_OBJFIRST); assert(r >= 0);
		r = engine->RegisterObjectMethod("Rigidbody", "void AddBoxCollider(const vec3& in)", asMETHOD(Rigidbody, AddBoxCollider), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("Rigidbody", "void AddSphereCollider(float radius)", asMETHOD(Rigidbody, AddSphereCollider), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("Rigidbody", "vec3 GetPosition()", asMETHOD(Rigidbody, GetPosition), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("Rigidbody", "vec3 GetRotation()", asMETHOD(Rigidbody, GetRotation), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("Rigidbody", "bool GetKinematic()", asMETHOD(Rigidbody, GetKinematic), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("Rigidbody", "void SetKinematic(bool set)", asMETHOD(Rigidbody, SetKinematic), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("Rigidbody", "float GetMass()", asMETHOD(Rigidbody, GetMass), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("Rigidbody", "void SetMass(float mass)", asMETHOD(Rigidbody, SetMass), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("Rigidbody", "void ApplyForce(vec3)", asMETHOD(Rigidbody, ApplyForce), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("Rigidbody", "void ApplyForceAtPosition(vec3, vec3)", asMETHOD(Rigidbody, ApplyForceAtPosition), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("Rigidbody", "void ApplyTorque(vec3)", asMETHOD(Rigidbody, ApplyTorque), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("Rigidbody", "void SetCOG(vec3)", asMETHOD(Rigidbody, SetCOG), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("Rigidbody", "void EnableGravity(bool)", asMETHOD(Rigidbody, EnableGravity), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("Rigidbody", "void SetFriction(float)", asMETHOD(Rigidbody, SetFriction), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("Rigidbody", "void SetBounciness(float)", asMETHOD(Rigidbody, SetBounciness), asCALL_THISCALL); assert(r >= 0);


		r = engine->RegisterObjectType("PhysicsComponent", sizeof(PhysicsComponent), asOBJ_VALUE | asOBJ_POD); assert(r >= 0);
		r = engine->RegisterObjectProperty("PhysicsComponent", "vec3 cog", asOFFSET(PhysicsComponent,cog)); assert(r >= 0);
		r = engine->RegisterObjectProperty("PhysicsComponent", "bool useGravity", asOFFSET(PhysicsComponent,useGravity)); assert(r >= 0);
		r = engine->RegisterObjectProperty("PhysicsComponent", "float mass", asOFFSET(PhysicsComponent,mass)); assert(r >= 0);
		r = engine->RegisterObjectProperty("PhysicsComponent", "float friction", asOFFSET(PhysicsComponent,friction)); assert(r >= 0);
		r = engine->RegisterObjectProperty("PhysicsComponent", "float bounciness", asOFFSET(PhysicsComponent,bounciness)); assert(r >= 0);
		r = engine->RegisterObjectProperty("PhysicsComponent", "bool isKinematic", asOFFSET(PhysicsComponent,isKinematic)); assert(r >= 0);
		r = engine->RegisterObjectMethod("PhysicsComponent", "void ApplyForce(const vec3 &in)", asMETHOD(PhysicsComponent, ApplyForce), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("PhysicsComponent", "void ApplyForceAtPosition(const vec3 &in, const vec3 &in)", asMETHOD(PhysicsComponent, ApplyForceAtPosition), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("PhysicsComponent", "void ApplyTorque(const vec3 &in)", asMETHOD(PhysicsComponent, ApplyTorque), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectProperty("PhysicsComponent", "Rigidbody &context", asOFFSET(PhysicsComponent,context)); assert(r >= 0);

		r = engine->RegisterObjectType("MeshFilterComponent", sizeof(MeshFilterComponent), asOBJ_VALUE | asOBJ_POD); assert(r >= 0);
		r = engine->RegisterObjectProperty("MeshFilterComponent", "string resource", asOFFSET(MeshFilterComponent,path)); assert(r >= 0);

		r = engine->RegisterObjectType("MaterialComponent", sizeof(MeshFilterComponent), asOBJ_VALUE | asOBJ_POD); assert(r >= 0);
		r = engine->RegisterObjectProperty("MaterialComponent", "string resource", asOFFSET(MaterialComponent,path)); assert(r >= 0);


		r = engine->RegisterObjectType("Entity", sizeof(Entity), asOBJ_VALUE | asOBJ_POD); assert(r >= 0);
		r = engine->RegisterObjectMethod("Entity", "void Destroy()", asMETHOD(Entity, Destroy), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("Entity", "bool IsValid()", asMETHOD(Entity, IsValid), asCALL_THISCALL); assert(r >= 0);

		r = engine->RegisterObjectProperty("TransformComponent", "Entity parent", asOFFSET(TransformComponent,parent)); assert(r >= 0);

		// Get Component functions
		r = engine->RegisterObjectMethod("Entity", "TransformComponent& GetTransformComponent()", asFUNCTION(Entity_GetTransformComponent), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectMethod("Entity", "bool HasTransformComponent()", asFUNCTION(Entity_HasTransformComponent), asCALL_CDECL_OBJLAST); assert(r >= 0);

		r = engine->RegisterObjectMethod("Entity", "ScriptComponent& GetScriptComponent()", asFUNCTION(Entity_GetScriptComponent), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectMethod("Entity", "bool HasScriptComponent()", asFUNCTION(Entity_HasScriptComponent), asCALL_CDECL_OBJLAST); assert(r >= 0);

		r = engine->RegisterObjectMethod("Entity", "PhysicsComponent& GetPhysicsComponent()", asFUNCTION(Entity_GetPhysicsComponent), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectMethod("Entity", "bool HasPhysicsComponent()", asFUNCTION(Entity_HasPhysicsComponent), asCALL_CDECL_OBJLAST); assert(r >= 0);

		r = engine->RegisterObjectMethod("Entity", "MeshFilterComponent& GetMeshFilterComponent()", asFUNCTION(Entity_GetMeshFilterComponent), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectMethod("Entity", "bool HasMeshFilterComponent()", asFUNCTION(Entity_HasMeshFilterComponent), asCALL_CDECL_OBJLAST); assert(r >= 0);

		r = engine->RegisterObjectMethod("Entity", "MaterialComponent& GetMaterialComponent()", asFUNCTION(Entity_GetMaterialComponent), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectMethod("Entity", "bool HasMaterialComponent()", asFUNCTION(Entity_HasMaterialComponent), asCALL_CDECL_OBJLAST); assert(r >= 0);

		engine->SetDefaultNamespace("Input");

		r = engine->RegisterObjectType("Button", sizeof(Key), asOBJ_VALUE | asOBJ_POD); assert(r >= 0);
		r = engine->RegisterObjectProperty("Button", "bool pressed", asOFFSET(Key,pressed)); assert(r >= 0);

		r = engine->RegisterGlobalFunction("const Button& GetButton(const string& in)", asFUNCTION(Input::GetKey), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("void RegisterButton(const string &in, int)", asFUNCTION(Input::RegisterKey), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("vec2 GetMouseChange()", asFUNCTION(Input::GetMouseChange), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("vec2 GetScrollDelta()", asFUNCTION(Input::GetScrollDelta), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("vec2 GetMousePos()", asFUNCTION(Input::GetMousePos), asCALL_CDECL); assert(r >= 0);

		engine->SetDefaultNamespace("Console");
		r = engine->RegisterGlobalFunction("void Log(const string& in)", asFUNCTION(scriptPrint), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("void LogError(const string& in)", asFUNCTION(scriptLogError), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("void LogWarning(const string& in)", asFUNCTION(scriptLogWarning), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("void LogFatalError(const string& in)", asFUNCTION(scriptLogFatalError), asCALL_CDECL); assert(r >= 0);

	}
}
