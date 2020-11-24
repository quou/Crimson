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

#include "Register.h"

#include "Input.h"

#include <GLFW/glfw3.h>

namespace Crimson {
	static void MessageCallback(const asSMessageInfo* msg, void* param) {
		if (msg->type == asMSGTYPE_ERROR) {
			CR_LOG_ERROR("%s (%d, %d): %s", msg->section, msg->row, msg->col, msg->message);
		} else if (msg->type == asMSGTYPE_WARNING) {
			CR_LOG_WARNING("%s (%d, %d): %s", msg->section, msg->row, msg->col, msg->message);
		} else if (msg->type == asMSGTYPE_INFORMATION) {
			CR_LOG("%s (%d, %d): %s", msg->section, msg->row, msg->col, msg->message);
		}
	}

	static unsigned int memoryUsage;

	static int IncludeCallback(const char *include, const char *from, CScriptBuilder *builder, void *userParam) {
      if (std::string(include) == "Crimson" || std::string(include) == "Crimson.as") {
         builder->AddSectionFromMemory("Crimson", g_behaviourBase);
         return 0;
      }

		auto assetManager = (AssetManager*)userParam;

      return builder->AddSectionFromMemory(include, assetManager->LoadText(include).c_str());
   }

	static void* ScriptMalloc(size_t size) {
		char *buffer = (char *) malloc(size + sizeof(int)); //allocate sizeof(int) extra bytes
		if (buffer == NULL) {
			return NULL; // no memory!
		}

		memoryUsage += size;
		int *sizeBox = (int*)buffer;
		*sizeBox = size; //store the size in first sizeof(int) bytes!
		return buffer + sizeof(int); //return buffer after sizeof(int) bytes!
	}

	static void ScriptFree(void* pointer) {
		if (pointer == NULL) {
			return; //no free
		}

		char *buffer = (char*)pointer - sizeof(int); //get the start of the buffer
		int *sizeBox = (int*)buffer;
		memoryUsage -= *sizeBox;
		free(buffer);
	}

	unsigned int ScriptManager::GetMemoryUsageKilobytes() {
		return memoryUsage * 0.001f;
	}

	ScriptManager::ScriptManager() {
		asSetGlobalMemoryFunctions(ScriptMalloc, ScriptFree);

		m_asEngine = asCreateScriptEngine();
		m_asContext = m_asEngine->CreateContext();
		m_asEngine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);

		RegisterScript(m_asEngine);
	}

	ScriptManager::~ScriptManager() {
		for (auto obj : m_objects) {
			if (obj.second.first) {
				obj.second.first->Release();
			}
		}

		m_asContext->Release();
		m_asEngine->ShutDownAndRelease();
	}

	void ScriptManager::DeInitScript(unsigned int id) {
		if (m_objects[id].first) {
			m_objects[id].first->Release();
			m_objects[id].first = NULL;
			m_objects.erase(m_objects.find(id));
		}
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

		m_updateTime = (float)glfwGetTime() - timeSinceStart;
	}

	void ScriptManager::PhysicsUpdate(float delta) {
		float timeSinceStart = glfwGetTime();

		if (!m_compilationSuccess) {return;}

		for (auto obj : m_objects) {
			Entity* ent = (Entity*)obj.second.first->GetAddressOfProperty(0);
			if (!ent->IsValid()) {
				obj.second.first->Release();
				m_objects.erase(obj.first);
				continue;
			}

			asIScriptFunction* func = obj.second.second->GetMethodByDecl("void OnPhysicsUpdate(float)");
			if (!func) {
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
