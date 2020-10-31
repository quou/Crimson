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


namespace Crimson {
	/* A base class that behaviour scripts will inherit from */
	static const char* g_behaviourBase = R"(
		abstract class CrimsonBehaviour {
			Entity m_entity;

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

	static TransformComponent& Entity_GetTransformComponent(Entity* ent) {
		return ent->GetComponent<TransformComponent>();
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
		r = m_asEngine->RegisterGlobalFunction("string to_string(vec2)", asFUNCTIONPR(scriptToString, (glm::vec2), std::string), asCALL_CDECL); assert(r >= 0);
		r = m_asEngine->RegisterGlobalFunction("string to_string(vec3)", asFUNCTIONPR(scriptToString, (glm::vec3), std::string), asCALL_CDECL); assert(r >= 0);
		r = m_asEngine->RegisterGlobalFunction("string to_string(vec4)", asFUNCTIONPR(scriptToString, (glm::vec4), std::string), asCALL_CDECL); assert(r >= 0);

		r = m_asEngine->RegisterObjectType("TransformComponent", sizeof(TransformComponent), asOBJ_VALUE | asOBJ_POD); assert(r >= 0);
      r = m_asEngine->RegisterObjectProperty("TransformComponent", "vec3 position", asOFFSET(TransformComponent,position)); assert(r >= 0);
      r = m_asEngine->RegisterObjectProperty("TransformComponent", "vec3 scale", asOFFSET(TransformComponent,scale)); assert(r >= 0);
      r = m_asEngine->RegisterObjectProperty("TransformComponent", "vec3 rotation", asOFFSET(TransformComponent,rotation)); assert(r >= 0);

		r = m_asEngine->RegisterObjectType("Entity", sizeof(Entity), asOBJ_VALUE | asOBJ_POD); assert(r >= 0);
		r = m_asEngine->RegisterObjectMethod("Entity", "TransformComponent& GetTransformComponent()", asFUNCTION(Entity_GetTransformComponent), asCALL_CDECL_OBJLAST); assert(r >= 0);
	}

	ScriptManager::~ScriptManager() {
		for (auto obj : m_objects) {
			obj.first->Release();
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
			return;
		}


		m_asModule = m_asEngine->GetModule("CrimsonBehaviours");

		m_compilationSuccess = true;

		Flush();
	}

	void ScriptManager::SetupEntity(entt::entity ent, Scene* scene) {
		Entity e(ent, scene);
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

		m_objects.push_back({obj, type});
	}

	void ScriptManager::Init() {
		if (!m_compilationSuccess) {return;}

		for (auto obj : m_objects) {
			asIScriptFunction* func = obj.second->GetMethodByDecl("void OnInit()");
			if (!func) {
				CR_LOG_WARNING("%s", "No 'void OnInit' function");
				continue;
			}

			m_asContext->Prepare(func);
			m_asContext->SetObject(obj.first);
			int r = m_asContext->Execute();

			if (r == asEXECUTION_EXCEPTION) {
				CR_LOG_ERROR("An exception '%s' occurred.", m_asContext->GetExceptionString());
				continue;
			}
		}
	}

	void ScriptManager::Update(float delta) {
		if (!m_compilationSuccess) {return;}

		for (auto obj : m_objects) {
			asIScriptFunction* func = obj.second->GetMethodByDecl("void OnUpdate(float)");
			if (!func) {
				CR_LOG_WARNING("%s", "No 'void OnUpdate(float)' function");
				continue;
			}

			m_asContext->Prepare(func);
			m_asContext->SetArgFloat(0, delta);
			m_asContext->SetObject(obj.first);
			int r = m_asContext->Execute();

			if (r == asEXECUTION_EXCEPTION) {
				CR_LOG_ERROR("An exception '%s' occurred.", m_asContext->GetExceptionString());
				continue;
			}
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
