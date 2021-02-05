#pragma once

#include <map>

#include "core.h"
#include "entity/entity.h"
#include "scripting/scriptmanager.h"

namespace Crimson {
	/* Controls an AngelScript behaviour */
	class CR_API ScriptComponent : public Component {
	private:
		BehaviourInstance m_behaviour;

	public:
		std::string m_behaviourDecl;
		std::map<BehaviourField, float> m_serialisableFloats;
		std::map<BehaviourField, int> m_serialisableInts;
		std::map<BehaviourField, std::string> m_serialisableStrings;

		bool m_dontCallInit = false;

		ScriptComponent(const char* behaviourDecl);

		void OnInit();
		void OnUpdate(float delta);
		void OnDestroy();

		const BehaviourInstance& GetInstance() const { return m_behaviour; }

		void ClearSerialisableValues();
	};
}
