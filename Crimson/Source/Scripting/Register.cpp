#include "Register.h"

#include <angelscript/angelscript.h>
#include <scriptstdstring/scriptstdstring.h>
#include <scriptbuilder/scriptbuilder.h>

#include <glm/glm.hpp>

#include "Logger.h"
#include "AssetManager.h"
#include "SceneManagement/Components.h"
#include "SceneManagement/Entity.h"
#include "SceneManagement/Scene.h"

#include "Input.h"

namespace Crimson {
	static void scriptPrint(std::string &msg)	{
		CR_LOG("%s", msg.c_str());
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
      r = engine->RegisterObjectMethod(asTypenamePtr, std::string(asTypename + "& opEquals(const " + asTypename + "& in) const").c_str(), asFUNCTIONPR(glm::operator/, (const T&, const T&), T), asCALL_CDECL_OBJLAST); assert( r >= 0 );
   }

	void RegisterScript(asIScriptEngine* engine) {
		RegisterStdString(engine);

		int r;

		r = engine->RegisterGlobalFunction("void print(const string& in)", asFUNCTION(scriptPrint), asCALL_CDECL); assert(r >= 0);

		RegisterVector<glm::vec2>(engine, "vec2");
      r = engine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(vec2_Constructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
      r = engine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f(float)", asFUNCTION(vec2_ConstructorXY), asCALL_CDECL_OBJLAST); assert( r >= 0 );
      r = engine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f(float, float)", asFUNCTION(vec2_ConstructorX_Y), asCALL_CDECL_OBJLAST); assert( r >= 0 );
      r = engine->RegisterObjectProperty("vec2", "float x", asOFFSET(glm::vec2,x)); assert( r >= 0 );
      r = engine->RegisterObjectProperty("vec2", "float y", asOFFSET(glm::vec2,y)); assert( r >= 0 );

      RegisterVector<glm::vec3>(engine, "vec3");
      r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(vec3_Constructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
      r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(float)", asFUNCTION(vec3_ConstructorXYZ), asCALL_CDECL_OBJLAST); assert( r >= 0 );
      r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(float, float, float)", asFUNCTION(vec3_ConstructorX_Y_Z), asCALL_CDECL_OBJLAST); assert( r >= 0 );
      r = engine->RegisterObjectProperty("vec3", "float x", asOFFSET(glm::vec3,x)); assert( r >= 0 );
      r = engine->RegisterObjectProperty("vec3", "float y", asOFFSET(glm::vec3,y)); assert( r >= 0 );
      r = engine->RegisterObjectProperty("vec3", "float z", asOFFSET(glm::vec3,z)); assert( r >= 0 );

      RegisterVector<glm::vec4>(engine, "vec4");
      r = engine->RegisterObjectBehaviour("vec4", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(vec4_Constructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
      r = engine->RegisterObjectBehaviour("vec4", asBEHAVE_CONSTRUCT, "void f(float)", asFUNCTION(vec4_ConstructorXYZW), asCALL_CDECL_OBJLAST); assert( r >= 0 );
      r = engine->RegisterObjectBehaviour("vec4", asBEHAVE_CONSTRUCT, "void f(float, float, float, float)", asFUNCTION(vec4_ConstructorX_Y_Z_W), asCALL_CDECL_OBJLAST); assert( r >= 0 );
      r = engine->RegisterObjectProperty("vec4", "float x", asOFFSET(glm::vec4,x)); assert( r >= 0 );
      r = engine->RegisterObjectProperty("vec4", "float y", asOFFSET(glm::vec4,y)); assert( r >= 0 );
      r = engine->RegisterObjectProperty("vec4", "float z", asOFFSET(glm::vec4,z)); assert( r >= 0 );
      r = engine->RegisterObjectProperty("vec4", "float w", asOFFSET(glm::vec4,w)); assert( r >= 0 );

		r = engine->RegisterGlobalFunction("string to_string(double)", asFUNCTIONPR(scriptToString, (double), std::string), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("string to_string(bool)", asFUNCTIONPR(scriptToString, (bool), std::string), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("string to_string(vec2)", asFUNCTIONPR(scriptToString, (glm::vec2), std::string), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("string to_string(vec3)", asFUNCTIONPR(scriptToString, (glm::vec3), std::string), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("string to_string(vec4)", asFUNCTIONPR(scriptToString, (glm::vec4), std::string), asCALL_CDECL); assert(r >= 0);

		r = engine->RegisterObjectType("TransformComponent", sizeof(TransformComponent), asOBJ_VALUE | asOBJ_POD); assert(r >= 0);
      r = engine->RegisterObjectProperty("TransformComponent", "vec3 position", asOFFSET(TransformComponent,position)); assert(r >= 0);
      r = engine->RegisterObjectProperty("TransformComponent", "vec3 scale", asOFFSET(TransformComponent,scale)); assert(r >= 0);
      r = engine->RegisterObjectProperty("TransformComponent", "vec3 rotation", asOFFSET(TransformComponent,rotation)); assert(r >= 0);
      r = engine->RegisterObjectProperty("TransformComponent", "string tag", asOFFSET(TransformComponent,tag)); assert(r >= 0);
      r = engine->RegisterObjectProperty("TransformComponent", "string name", asOFFSET(TransformComponent,name)); assert(r >= 0);

		r = engine->RegisterObjectType("ScriptComponent", sizeof(ScriptComponent), asOBJ_VALUE | asOBJ_POD); assert(r >= 0);

		r = engine->RegisterObjectType("Rigidbody", sizeof(Rigidbody), asOBJ_VALUE); assert(r >= 0);
		r = engine->RegisterObjectBehaviour("Rigidbody", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Rigidbody_Constructor), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectBehaviour("Rigidbody", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(Rigidbody_Destructor), asCALL_CDECL_OBJLAST); assert(r >= 0);
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


		r = engine->RegisterObjectType("Entity", sizeof(Entity), asOBJ_VALUE | asOBJ_POD); assert(r >= 0);
		r = engine->RegisterObjectMethod("Entity", "void Destroy()", asMETHOD(Entity, Destroy), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("Entity", "bool IsValid()", asMETHOD(Entity, IsValid), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod("Entity", "TransformComponent& GetTransformComponent()", asFUNCTION(Entity_GetTransformComponent), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectMethod("Entity", "ScriptComponent& GetScriptComponent()", asFUNCTION(Entity_GetScriptComponent), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectMethod("Entity", "PhysicsComponent& GetPhysicsComponent()", asFUNCTION(Entity_GetPhysicsComponent), asCALL_CDECL_OBJLAST); assert(r >= 0);

		engine->SetDefaultNamespace("Input");

		r = engine->RegisterObjectType("Button", sizeof(Key), asOBJ_VALUE | asOBJ_POD); assert(r >= 0);
		r = engine->RegisterObjectProperty("Button", "bool pressed", asOFFSET(Key,pressed)); assert(r >= 0);

		r = engine->RegisterGlobalFunction("const Button& GetButton(string)", asFUNCTION(Input::GetKey), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("vec2 GetMouseChange()", asFUNCTION(Input::GetMouseChange), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("vec2 GetScrollDelta()", asFUNCTION(Input::GetScrollDelta), asCALL_CDECL); assert(r >= 0);
		r = engine->RegisterGlobalFunction("vec2 GetMousePos()", asFUNCTION(Input::GetMousePos), asCALL_CDECL); assert(r >= 0);
	}
}
