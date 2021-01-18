#include <angelscript/angelscript.h>

#include "script.h"
#include "entity/scene.h"

namespace Crimson {
	ScriptComponent::ScriptComponent(const char* behaviourDecl) :
	       	m_behaviourDecl(behaviourDecl) {}

	void ScriptComponent::OnInit() {
		/* Create an instance of the object with the class name */
		m_behaviour = m_entity->m_scene->m_scriptManager->
			CreateInstance(m_behaviourDecl.c_str(), m_entity);

		/* Call the script's OnInit function */
		m_entity->m_scene->m_scriptManager->CallInit(m_behaviour);
	}

	void ScriptComponent::OnUpdate(float delta) {
		/* Call the script's OnUpdate function */
		m_entity->m_scene->m_scriptManager->CallUpdate(m_behaviour, delta);	
	}

	void ScriptComponent::OnDestroy() {
		/* This is kind of ugly. It's to prevent a double free, if for some
		 * reason this function is called more than once (which can happen). */
		if (m_behaviour.instance) {
			m_behaviour.instance->Release();
			m_behaviour.instance = NULL;
		}
	}
}
