#include "ScriptManager.h"

#include <angelscript/angelscript.h>
#include <scriptstdstring/scriptstdstring.h>
#include <scriptbuilder/scriptbuilder.h>

#include <cassert>

#include "Logger.h"
#include "AssetManager.h"
#include "SceneManagement/Components.h"
#include "SceneManagement/Entity.h"
#include "SceneManagement/Scene.h"

#include "Input.h"

#include <GLFW/glfw3.h>

namespace Crimson {
	/* A base class that behaviour scripts will inherit from */
	static const char* g_behaviourBase = R"(
		abstract class CrimsonBehaviour {
			Entity m_entity;

			void Destroy() {
				m_entity.Destroy();
			}

			TransformComponent& GetTransformComponent() {
				return m_entity.GetTransformComponent();
			}

			ScriptComponent& GetScriptComponent() {
				return m_entity.GetScriptComponent();
			}

			PhysicsComponent& GetPhysicsComponent() {
				return m_entity.GetPhysicsComponent();
			}

			void OnInit() {}
			void OnUpdate(float delta) {}
		}
	)";

	static void MessageCallback(const asSMessageInfo* msg, void* param) {
		if (msg->type == asMSGTYPE_ERROR) {
			CR_LOG_ERROR("%s (%d, %d): %s", msg->section, msg->row, msg->col, msg->message);
		} else if (msg->type == asMSGTYPE_WARNING) {
			CR_LOG_WARNING("%s (%d, %d): %s", msg->section, msg->row, msg->col, msg->message);
		} else if (msg->type == asMSGTYPE_INFORMATION) {
			CR_LOG("%s (%d, %d): %s", msg->section, msg->row, msg->col, msg->message);
		}
	}

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

	static int IncludeCallback(const char *include, const char *from, CScriptBuilder *builder, void *userParam) {
      if (std::string(include) == "Crimson" || std::string(include) == "Crimson.as") {
         builder->AddSectionFromMemory("Crimson", g_behaviourBase);
         return 0;
      }

		auto assetManager = (AssetManager*)userParam;

      return builder->AddSectionFromMemory(include, assetManager->LoadText(include).c_str());
   }

	ScriptManager::ScriptManager() {
		m_asEngine = asCreateScriptEngine();
		m_asContext = m_asEngine->CreateContext();
		m_asEngine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);

		RegisterStdString(m_asEngine);

		int r;

		r = m_asEngine->RegisterGlobalFunction("void print(const string& in)", asFUNCTION(scriptPrint), asCALL_CDECL); assert(r >= 0);

		RegisterVector<glm::vec2>(m_asEngine, "vec2");
      r = m_asEngine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(vec2_Constructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
      r = m_asEngine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f(float)", asFUNCTION(vec2_ConstructorXY), asCALL_CDECL_OBJLAST); assert( r >= 0 );
      r = m_asEngine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f(float, float)", asFUNCTION(vec2_ConstructorX_Y), asCALL_CDECL_OBJLAST); assert( r >= 0 );
      r = m_asEngine->RegisterObjectProperty("vec2", "float x", asOFFSET(glm::vec2,x)); assert( r >= 0 );
      r = m_asEngine->RegisterObjectProperty("vec2", "float y", asOFFSET(glm::vec2,y)); assert( r >= 0 );

      RegisterVector<glm::vec3>(m_asEngine, "vec3");
      r = m_asEngine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(vec3_Constructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
      r = m_asEngine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(float)", asFUNCTION(vec3_ConstructorXYZ), asCALL_CDECL_OBJLAST); assert( r >= 0 );
      r = m_asEngine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(float, float, float)", asFUNCTION(vec3_ConstructorX_Y_Z), asCALL_CDECL_OBJLAST); assert( r >= 0 );
      r = m_asEngine->RegisterObjectProperty("vec3", "float x", asOFFSET(glm::vec3,x)); assert( r >= 0 );
      r = m_asEngine->RegisterObjectProperty("vec3", "float y", asOFFSET(glm::vec3,y)); assert( r >= 0 );
      r = m_asEngine->RegisterObjectProperty("vec3", "float z", asOFFSET(glm::vec3,z)); assert( r >= 0 );

      RegisterVector<glm::vec4>(m_asEngine, "vec4");
      r = m_asEngine->RegisterObjectBehaviour("vec4", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(vec4_Constructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
      r = m_asEngine->RegisterObjectBehaviour("vec4", asBEHAVE_CONSTRUCT, "void f(float)", asFUNCTION(vec4_ConstructorXYZW), asCALL_CDECL_OBJLAST); assert( r >= 0 );
      r = m_asEngine->RegisterObjectBehaviour("vec4", asBEHAVE_CONSTRUCT, "void f(float, float, float, float)", asFUNCTION(vec4_ConstructorX_Y_Z_W), asCALL_CDECL_OBJLAST); assert( r >= 0 );
      r = m_asEngine->RegisterObjectProperty("vec4", "float x", asOFFSET(glm::vec4,x)); assert( r >= 0 );
      r = m_asEngine->RegisterObjectProperty("vec4", "float y", asOFFSET(glm::vec4,y)); assert( r >= 0 );
      r = m_asEngine->RegisterObjectProperty("vec4", "float z", asOFFSET(glm::vec4,z)); assert( r >= 0 );
      r = m_asEngine->RegisterObjectProperty("vec4", "float w", asOFFSET(glm::vec4,w)); assert( r >= 0 );

		r = m_asEngine->RegisterGlobalFunction("string to_string(double)", asFUNCTIONPR(scriptToString, (double), std::string), asCALL_CDECL); assert(r >= 0);
		r = m_asEngine->RegisterGlobalFunction("string to_string(bool)", asFUNCTIONPR(scriptToString, (bool), std::string), asCALL_CDECL); assert(r >= 0);
		r = m_asEngine->RegisterGlobalFunction("string to_string(vec2)", asFUNCTIONPR(scriptToString, (glm::vec2), std::string), asCALL_CDECL); assert(r >= 0);
		r = m_asEngine->RegisterGlobalFunction("string to_string(vec3)", asFUNCTIONPR(scriptToString, (glm::vec3), std::string), asCALL_CDECL); assert(r >= 0);
		r = m_asEngine->RegisterGlobalFunction("string to_string(vec4)", asFUNCTIONPR(scriptToString, (glm::vec4), std::string), asCALL_CDECL); assert(r >= 0);

		r = m_asEngine->RegisterObjectType("TransformComponent", sizeof(TransformComponent), asOBJ_VALUE | asOBJ_POD); assert(r >= 0);
      r = m_asEngine->RegisterObjectProperty("TransformComponent", "vec3 position", asOFFSET(TransformComponent,position)); assert(r >= 0);
      r = m_asEngine->RegisterObjectProperty("TransformComponent", "vec3 scale", asOFFSET(TransformComponent,scale)); assert(r >= 0);
      r = m_asEngine->RegisterObjectProperty("TransformComponent", "vec3 rotation", asOFFSET(TransformComponent,rotation)); assert(r >= 0);
      r = m_asEngine->RegisterObjectProperty("TransformComponent", "string tag", asOFFSET(TransformComponent,tag)); assert(r >= 0);
      r = m_asEngine->RegisterObjectProperty("TransformComponent", "string name", asOFFSET(TransformComponent,name)); assert(r >= 0);

		r = m_asEngine->RegisterObjectType("ScriptComponent", sizeof(ScriptComponent), asOBJ_VALUE | asOBJ_POD); assert(r >= 0);

		r = m_asEngine->RegisterObjectType("Rigidbody", sizeof(Rigidbody), asOBJ_VALUE); assert(r >= 0);
		r = m_asEngine->RegisterObjectBehaviour("Rigidbody", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Rigidbody_Constructor), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = m_asEngine->RegisterObjectBehaviour("Rigidbody", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(Rigidbody_Destructor), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = m_asEngine->RegisterObjectMethod("Rigidbody", "void AddBoxCollider(const vec3& in)", asMETHOD(Rigidbody, AddBoxCollider), asCALL_THISCALL); assert(r >= 0);
		r = m_asEngine->RegisterObjectMethod("Rigidbody", "void AddSphereCollider(float radius)", asMETHOD(Rigidbody, AddSphereCollider), asCALL_THISCALL); assert(r >= 0);
		r = m_asEngine->RegisterObjectMethod("Rigidbody", "vec3 GetPosition()", asMETHOD(Rigidbody, GetPosition), asCALL_THISCALL); assert(r >= 0);
		r = m_asEngine->RegisterObjectMethod("Rigidbody", "vec3 GetRotation()", asMETHOD(Rigidbody, GetRotation), asCALL_THISCALL); assert(r >= 0);
		r = m_asEngine->RegisterObjectMethod("Rigidbody", "bool GetKinematic()", asMETHOD(Rigidbody, GetKinematic), asCALL_THISCALL); assert(r >= 0);
		r = m_asEngine->RegisterObjectMethod("Rigidbody", "void SetKinematic(bool set)", asMETHOD(Rigidbody, SetKinematic), asCALL_THISCALL); assert(r >= 0);
		r = m_asEngine->RegisterObjectMethod("Rigidbody", "float GetMass()", asMETHOD(Rigidbody, GetMass), asCALL_THISCALL); assert(r >= 0);
		r = m_asEngine->RegisterObjectMethod("Rigidbody", "void SetMass(float mass)", asMETHOD(Rigidbody, SetMass), asCALL_THISCALL); assert(r >= 0);
		r = m_asEngine->RegisterObjectMethod("Rigidbody", "void ApplyForce(vec3)", asMETHOD(Rigidbody, ApplyForce), asCALL_THISCALL); assert(r >= 0);
		r = m_asEngine->RegisterObjectMethod("Rigidbody", "void ApplyForceAtPosition(vec3, vec3)", asMETHOD(Rigidbody, ApplyForceAtPosition), asCALL_THISCALL); assert(r >= 0);
		r = m_asEngine->RegisterObjectMethod("Rigidbody", "void ApplyTorque(vec3)", asMETHOD(Rigidbody, ApplyTorque), asCALL_THISCALL); assert(r >= 0);
		r = m_asEngine->RegisterObjectMethod("Rigidbody", "void SetCOG(vec3)", asMETHOD(Rigidbody, SetCOG), asCALL_THISCALL); assert(r >= 0);
		r = m_asEngine->RegisterObjectMethod("Rigidbody", "void EnableGravity(bool)", asMETHOD(Rigidbody, EnableGravity), asCALL_THISCALL); assert(r >= 0);
		r = m_asEngine->RegisterObjectMethod("Rigidbody", "void SetFriction(float)", asMETHOD(Rigidbody, SetFriction), asCALL_THISCALL); assert(r >= 0);
		r = m_asEngine->RegisterObjectMethod("Rigidbody", "void SetBounciness(float)", asMETHOD(Rigidbody, SetBounciness), asCALL_THISCALL); assert(r >= 0);


		r = m_asEngine->RegisterObjectType("PhysicsComponent", sizeof(PhysicsComponent), asOBJ_VALUE | asOBJ_POD); assert(r >= 0);
		r = m_asEngine->RegisterObjectProperty("PhysicsComponent", "vec3 cog", asOFFSET(PhysicsComponent,cog)); assert(r >= 0);
		r = m_asEngine->RegisterObjectProperty("PhysicsComponent", "bool useGravity", asOFFSET(PhysicsComponent,useGravity)); assert(r >= 0);
		r = m_asEngine->RegisterObjectProperty("PhysicsComponent", "float mass", asOFFSET(PhysicsComponent,mass)); assert(r >= 0);
		r = m_asEngine->RegisterObjectProperty("PhysicsComponent", "float friction", asOFFSET(PhysicsComponent,friction)); assert(r >= 0);
		r = m_asEngine->RegisterObjectProperty("PhysicsComponent", "float bounciness", asOFFSET(PhysicsComponent,bounciness)); assert(r >= 0);
		r = m_asEngine->RegisterObjectProperty("PhysicsComponent", "bool isKinematic", asOFFSET(PhysicsComponent,isKinematic)); assert(r >= 0);
		r = m_asEngine->RegisterObjectMethod("PhysicsComponent", "void ApplyForce(const vec3 &in)", asMETHOD(PhysicsComponent, ApplyForce), asCALL_THISCALL); assert(r >= 0);
		r = m_asEngine->RegisterObjectMethod("PhysicsComponent", "void ApplyForceAtPosition(const vec3 &in, const vec3 &in)", asMETHOD(PhysicsComponent, ApplyForceAtPosition), asCALL_THISCALL); assert(r >= 0);
		r = m_asEngine->RegisterObjectMethod("PhysicsComponent", "void ApplyTorque(const vec3 &in)", asMETHOD(PhysicsComponent, ApplyTorque), asCALL_THISCALL); assert(r >= 0);
		r = m_asEngine->RegisterObjectProperty("PhysicsComponent", "Rigidbody &context", asOFFSET(PhysicsComponent,context)); assert(r >= 0);


		r = m_asEngine->RegisterObjectType("Entity", sizeof(Entity), asOBJ_VALUE | asOBJ_POD); assert(r >= 0);
		r = m_asEngine->RegisterObjectMethod("Entity", "void Destroy()", asMETHOD(Entity, Destroy), asCALL_THISCALL); assert(r >= 0);
		r = m_asEngine->RegisterObjectMethod("Entity", "bool IsValid()", asMETHOD(Entity, IsValid), asCALL_THISCALL); assert(r >= 0);
		r = m_asEngine->RegisterObjectMethod("Entity", "TransformComponent& GetTransformComponent()", asFUNCTION(Entity_GetTransformComponent), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = m_asEngine->RegisterObjectMethod("Entity", "ScriptComponent& GetScriptComponent()", asFUNCTION(Entity_GetScriptComponent), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = m_asEngine->RegisterObjectMethod("Entity", "PhysicsComponent& GetPhysicsComponent()", asFUNCTION(Entity_GetPhysicsComponent), asCALL_CDECL_OBJLAST); assert(r >= 0);

		m_asEngine->SetDefaultNamespace("Input");

		r = m_asEngine->RegisterObjectType("Button", sizeof(Key), asOBJ_VALUE | asOBJ_POD); assert(r >= 0);
		r = m_asEngine->RegisterObjectProperty("Button", "bool pressed", asOFFSET(Key,pressed)); assert(r >= 0);

		r = m_asEngine->RegisterGlobalFunction("const Button& GetButton(string)", asFUNCTION(Input::GetKey), asCALL_CDECL); assert(r >= 0);
	}

	ScriptManager::~ScriptManager() {
		for (auto obj : m_objects) {
			obj.second.first->Release();
		}

		m_asContext->Release();
		m_asEngine->ShutDownAndRelease();
	}

	void ScriptManager::Compile(AssetManager& assetManager) {
		m_compilationSuccess = false;

		CScriptBuilder builder;

		builder.SetIncludeCallback(IncludeCallback, &assetManager);

		int r = builder.StartNewModule(m_asEngine, "CrimsonBehaviours");
		if (r < 0) {
			CR_LOG_ERROR("%s", "Unrecoverable error starting new AngelScript module");
			return;
		}

		int i = 0;
		for (auto& s : m_codeFiles) {
			i++;

			int r = builder.AddSectionFromMemory(std::to_string(i).c_str(), s.c_str());
			if (r < 0) {
				continue;
			}
		}

		r = builder.BuildModule();
		if (r < 0) {
			// Erorr building
			Flush();
			return;
		}

		m_asModule = m_asEngine->GetModule("CrimsonBehaviours");

		m_compilationSuccess = true;

		Flush();
	}

	void ScriptManager::SetupEntity(entt::entity ent, Scene* scene) {
		if (!m_compilationSuccess) {return;}
		m_currentID++;

		Entity e(ent, scene);

		if (e.GetComponent<ScriptComponent>().className == "-") {
			return;
		}

		e.GetComponent<ScriptComponent>().id = m_currentID;
		std::string c = e.GetComponent<ScriptComponent>().className.c_str();

		asITypeInfo* type = m_asModule->GetTypeInfoByDecl(c.c_str());
		if (!type) {
			CR_LOG_ERROR("Class '%s' doesn't exist in the current module", c.c_str());
			return;
		}

		asIScriptFunction* factory = type->GetFactoryByDecl(std::string(c + " @" + c + "()").c_str());
		if (!factory) {
			CR_LOG_ERROR("Class '%s' doesn't exist in the current module", c.c_str());
			return;
		}

		m_asContext->Prepare(factory);
		m_asContext->Execute();

		asIScriptObject* obj = *(asIScriptObject**)m_asContext->GetAddressOfReturnValue();
		*(Entity*)obj->GetAddressOfProperty(0) = e;
		obj->AddRef();

		m_objects[m_currentID] = {obj, type};
	}

	void ScriptManager::Init() {
		if (!m_compilationSuccess) {return;}

		for (auto obj : m_objects) {
			Entity* ent = (Entity*)obj.second.first->GetAddressOfProperty(0);
			if (!ent->IsValid()) {
				obj.second.first->Release();
				m_objects.erase(obj.first);
				continue;
			}

			asIScriptFunction* func = obj.second.second->GetMethodByDecl("void OnInit()");
			if (!func) {
				CR_LOG_WARNING("%s", "No 'void OnInit' function");
				continue;
			}

			m_asContext->Prepare(func);
			m_asContext->SetObject(obj.second.first);
			int r = m_asContext->Execute();

			if (r == asEXECUTION_EXCEPTION) {
				CR_LOG_ERROR("An exception '%s' occurred.", m_asContext->GetExceptionString());
				continue;
			}
		}
	}

	void ScriptManager::Update(float delta) {
		float timeSinceStart = glfwGetTime();

		if (!m_compilationSuccess) {return;}

		for (auto obj : m_objects) {
			Entity* ent = (Entity*)obj.second.first->GetAddressOfProperty(0);
			if (!ent->IsValid()) {
				obj.second.first->Release();
				m_objects.erase(obj.first);
				continue;
			}

			asIScriptFunction* func = obj.second.second->GetMethodByDecl("void OnUpdate(float)");
			if (!func) {
				CR_LOG_WARNING("%s", "No 'void OnUpdate(float)' function");
				continue;
			}

			m_asContext->Prepare(func);
			m_asContext->SetArgFloat(0, delta);
			m_asContext->SetObject(obj.second.first);
			int r = m_asContext->Execute();

			if (r == asEXECUTION_EXCEPTION) {
				CR_LOG_ERROR("An exception '%s' occurred.", m_asContext->GetExceptionString());
				continue;
			}
		}

		m_updateTime = timeSinceStart - m_oldTimeSinceStart;
		m_oldTimeSinceStart = timeSinceStart;
	}

	void ScriptManager::ContactStay(unsigned int id, Entity& other) {
		if (!m_compilationSuccess) {return;}

		if (m_objects.count(id) == 0) {
			CR_LOG_ERROR("%s", "Invalid script ID");
			return;
		}

		auto obj = m_objects[id];
		Entity* ent = (Entity*)obj.first->GetAddressOfProperty(0);
		if (!ent->IsValid()) {
			obj.second->Release();
			m_objects.erase(id);
			return;
		}

		asIScriptFunction* func = obj.second->GetMethodByDecl("void OnContactStay(Entity)");
		if (!func) {
			return;
		}

		m_asContext->Prepare(func);
		m_asContext->SetArgObject(0, &other);
		m_asContext->SetObject(obj.first);
		int r = m_asContext->Execute();

		if (r == asEXECUTION_EXCEPTION) {
			CR_LOG_ERROR("An exception '%s' occurred.", m_asContext->GetExceptionString());
			return;
		}
	}

	void ScriptManager::ContactEnter(unsigned int id, Entity& other) {
		if (!m_compilationSuccess) {return;}

		if (m_objects.count(id) == 0) {
			CR_LOG_ERROR("%s", "Invalid script ID");
			return;
		}

		auto obj = m_objects[id];
		Entity* ent = (Entity*)obj.first->GetAddressOfProperty(0);
		if (!ent->IsValid()) {
			obj.second->Release();
			m_objects.erase(id);
			return;
		}

		asIScriptFunction* func = obj.second->GetMethodByDecl("void OnContactEnter(Entity)");
		if (!func) {
			return;
		}

		m_asContext->Prepare(func);
		m_asContext->SetArgObject(0, &other);
		m_asContext->SetObject(obj.first);
		int r = m_asContext->Execute();

		if (r == asEXECUTION_EXCEPTION) {
			CR_LOG_ERROR("An exception '%s' occurred.", m_asContext->GetExceptionString());
			return;
		}
	}

	void ScriptManager::ContactExit(unsigned int id, Entity& other) {
		if (!m_compilationSuccess) {return;}

		if (m_objects.count(id) == 0) {
			CR_LOG_ERROR("%s", "Invalid script ID");
			return;
		}

		auto obj = m_objects[id];
		Entity* ent = (Entity*)obj.first->GetAddressOfProperty(0);
		if (!ent->IsValid()) {
			obj.second->Release();
			m_objects.erase(id);
			return;
		}

		asIScriptFunction* func = obj.second->GetMethodByDecl("void OnContactExit(Entity)");
		if (!func) {
			return;
		}

		m_asContext->Prepare(func);
		m_asContext->SetArgObject(0, &other);
		m_asContext->SetObject(obj.first);
		int r = m_asContext->Execute();

		if (r == asEXECUTION_EXCEPTION) {
			CR_LOG_ERROR("An exception '%s' occurred.", m_asContext->GetExceptionString());
			return;
		}
	}

	void ScriptManager::AddBehaviour(const std::string& className) {
		m_classNames.push_back(className);
	}

	void ScriptManager::AddScript(const std::string& code) {
		m_codeFiles.push_back(code);
	}

	void ScriptManager::Flush() {
		m_codeFiles.clear();
		m_classNames.clear();
	}
}
