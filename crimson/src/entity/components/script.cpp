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


		std::vector<BehaviourField> invalidFields;
		/* Set up serialisable fields */
		for (auto& f : m_serialisableFloats) {
			if (!m_entity->m_scene->m_scriptManager->SetFloatProperty(m_behaviour, f.first, f.second)) {
				invalidFields.push_back(f.first);
			}
		}

		for (auto& f : m_serialisableInts) {
			if (!m_entity->m_scene->m_scriptManager->SetIntProperty(m_behaviour, f.first, f.second)) {
				invalidFields.push_back(f.first);
			}
		}

		for (auto& f : m_serialisableStrings) {
			if (!m_entity->m_scene->m_scriptManager->SetStringProperty(m_behaviour, f.first, f.second)) {
				invalidFields.push_back(f.first);
			}
		}

		for (auto& f : invalidFields) {
			try {
				m_serialisableFloats.erase(f);
			} catch (const std::exception& e) {}

			try {
				m_serialisableInts.erase(f);
			} catch (const std::exception& e) {}

			try {
				m_serialisableStrings.erase(f);
			} catch (const std::exception& e) {}
		}

		if (!m_dontCallInit) {
			/* Call the script's OnInit function */
			m_entity->m_scene->m_scriptManager->CallInit(m_behaviour);
		}
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
