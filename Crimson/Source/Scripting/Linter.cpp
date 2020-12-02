#include "Linter.h"

#include <angelscript/angelscript.h>
#include <scriptbuilder/scriptbuilder.h>

#include "Logger.h"
#include "Register.h"

namespace Crimson {
	static void MessageCallback(const asSMessageInfo* msg, void* param) {
		auto linter = (Linter*)param;
		if (msg->type == asMSGTYPE_ERROR) {
			linter->AddMessage({msg->row, msg->col, msg->message});
		}
	}

	Linter::Linter() {
		m_asEngine = asCreateScriptEngine();

		m_asEngine->SetMessageCallback(asFUNCTION(MessageCallback), this, asCALL_CDECL);

		RegisterScript(m_asEngine);
	}

	Linter::~Linter() {
		m_asEngine->Release();
	}

	std::vector<LinterMessage>& Linter::Lint(const std::string& code) {
		m_messages.clear();

		CScriptBuilder builder;


		int r = builder.StartNewModule(m_asEngine, "CrimsonBehaviours");
		if (r < 0) {
			CR_LOG_ERROR("%s", "Unrecoverable error starting new AngelScript module");
			return m_messages;
		}

		builder.AddSectionFromMemory("CrimsonBase", g_behaviourBase);

		r = builder.AddSectionFromMemory("Main", code.c_str());
		if (r < 0) {
			return m_messages;
		}

		r = builder.BuildModule();
		if (r < 0) {
			// Erorr building
			return m_messages;
		}

		return m_messages;
	}
}
