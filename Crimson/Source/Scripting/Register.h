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

			void OnInit() {}
			void OnUpdate(float delta) {}
			void OnContactStay(Entity other) {}
			void OnContactEnter(Entity other) {}
			void OnContactExit(Entity other) {}
		}
	)";

	void RegisterScript(asIScriptEngine* engine);
}
