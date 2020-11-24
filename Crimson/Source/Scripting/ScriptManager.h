#pragma once

#include <vector>
#include <string>
#include <tuple>

#include <entt.hpp>

class asIScriptEngine;
class asIScriptContext;
class asIScriptModule;
class asIScriptObject;
class asITypeInfo;

namespace Crimson {
	struct ScriptComponent;
	class Entity;
	class AssetManager;
	class Scene;

	class ScriptManager {
	private:
		asIScriptEngine* m_asEngine;
		asIScriptContext* m_asContext;
		asIScriptModule* m_asModule;

		std::vector<std::string> m_codeFiles;
		std::vector<std::string> m_classNames;
		std::map<unsigned int, std::pair<asIScriptObject*, asITypeInfo*>> m_objects;

		bool m_compilationSuccess;

		unsigned int m_currentID{0};

		float m_updateTime{0.0f};
		float m_oldTimeSinceStart{0.0f};
	public:
		ScriptManager();
		~ScriptManager();

		void AddBehaviour(const std::string& className);
		void AddScript(const std::string& code);
		void Flush();

		void Compile(AssetManager& assetManager);
		void SetupEntity(entt::entity ent, Scene* scene);

		void DeInitScript(unsigned int id);

		inline float GetUpdateTime() {return m_updateTime;}

		unsigned int GetMemoryUsageKilobytes();

		void Init();
		void Update(float delta);
		void PhysicsUpdate(float delta);
		void ContactStay(unsigned int id, Entity& other);
		void ContactEnter(unsigned int id, Entity& other);
		void ContactExit(unsigned int id, Entity& other);
	};
}
