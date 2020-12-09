#include "Linter.h"

#include <angelscript/angelscript.h>
#include <scriptbuilder/scriptbuilder.h>

#include "Logger.h"
#include "Register.h"

#include <filesystem>

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
		m_classes.clear();

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
			// Error building
			return m_messages;
		}

		m_asModule = m_asEngine->GetModule("CrimsonBehaviours");

		m_baseTypeInfo = m_asModule->GetTypeInfoByDecl("CrimsonBehaviour");

		for (int i = 0; i < m_asModule->GetObjectTypeCount(); i++) {
			auto info = m_asModule->GetObjectTypeByIndex(i);

			if (info->DerivesFrom(m_baseTypeInfo)) {
				m_classes.push_back(info->GetName());
			}
		}

		return m_messages;
	}

	std::vector<LinterMessage>& Linter::LintDir(const std::string& dir) {
		m_messages.clear();
		m_classes.clear();

		CScriptBuilder builder;

		int r = builder.StartNewModule(m_asEngine, "CrimsonBehaviours");
		if (r < 0) {
			CR_LOG_ERROR("%s", "Unrecoverable error starting new AngelScript module");
			return m_messages;
		}

		builder.AddSectionFromMemory("CrimsonBase", g_behaviourBase);

		try {
			for (const auto& entry : std::filesystem::recursive_directory_iterator(dir)) {
				if (!entry.is_directory() && entry.path().extension().string() == ".as") {
					builder.AddSectionFromFile(entry.path().string().c_str());
				}
			}
		} catch (const std::filesystem::filesystem_error& e) {
			CR_LOG_ERROR("%s", e.what());
		}

		r = builder.BuildModule();
		if (r < 0) {
			// Error building
			return m_messages;
		}

		m_asModule = m_asEngine->GetModule("CrimsonBehaviours");

		m_baseTypeInfo = m_asModule->GetTypeInfoByDecl("CrimsonBehaviour");

		for (int i = 0; i < m_asModule->GetObjectTypeCount(); i++) {
			auto info = m_asModule->GetObjectTypeByIndex(i);

			if (info->DerivesFrom(m_baseTypeInfo)) {
				m_classes.push_back(info->GetName());
			}
		}

		return m_messages;
	}
}
