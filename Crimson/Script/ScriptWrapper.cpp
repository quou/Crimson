#include "ScriptWrapper.h"

#include <iostream>
#include <fstream>

namespace Crimson {
   ScriptWrapper::ScriptWrapper() {
      m_runtime = Jinx::CreateRuntime();
   }

   void ScriptWrapper::LoadAndCompile(const std::string& scriptFile) {
      m_codePath = scriptFile;

      std::string scriptText;
      std::ifstream file(scriptFile);
      if (file.is_open()) {
         std::string line;
         while (std::getline(file, line)) {
            scriptText+=line+"\n";
         }
         file.close();
      } else {
         std::cout << "Unable to open script file: " << scriptFile << '\n';
      }

      m_currentLibrary = m_runtime->GetLibrary("crimson");

      m_currentLibrary->RegisterFunction(Jinx::Visibility::Public, "meaning of life",
      [](Jinx::ScriptPtr script, Jinx::Parameters params)->Jinx::Variant {
         return 42;
      });

      m_bytecode = m_runtime->Compile(scriptText.c_str());

      if (!m_bytecode)
         return;

      m_script = m_runtime->CreateScript(m_bytecode);

      m_hasCompiled = true;
   }

   void ScriptWrapper::Execute() {
      if (m_hasCompiled) {
         m_script->Execute();
      }
   }

   void ScriptWrapper::RunFunction(const std::string& prototype, Jinx::Parameters params) {
      if (!m_hasCompiled) { return; }

      if (m_runtimeIDs.count(prototype) == 0) {
         m_runtimeIDs[prototype] = m_script->FindFunction(nullptr, prototype.c_str());
      }

      m_script->CallFunction(m_runtimeIDs[prototype], params);
   }

   void ScriptWrapper::RunFunction(const std::string& prototype) {
      if (!m_hasCompiled) { return; }

      if (m_runtimeIDs.count(prototype) == 0) {
         m_runtimeIDs[prototype] = m_script->FindFunction(nullptr, prototype.c_str());
      }

      m_script->CallFunction(m_runtimeIDs[prototype], {});
   }
}
