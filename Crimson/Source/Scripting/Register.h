#pragma once

class asIScriptEngine;

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

	void RegisterScript(asIScriptEngine* engine);
}
