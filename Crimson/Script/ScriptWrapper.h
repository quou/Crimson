#ifndef SCRIPTWRAPPER_H
#define SCRIPTWRAPPER_H

#include "Jinx.hpp"

#include <map>

namespace Crimson {
   class ScriptWrapper {
   private:
      Jinx::LibraryPtr m_currentLibrary;
      Jinx::RuntimePtr m_runtime;
      Jinx::ScriptPtr m_script;
      Jinx::BufferPtr m_bytecode;

      std::map<std::string, Jinx::RuntimeID> m_runtimeIDs;
   public:
      ScriptWrapper();
      void LoadAndCompile(const std::string& scriptFile);
      void Execute();
      void RunFunction(const std::string& prototype, Jinx::Parameters params);
      void RunFunction(const std::string& prototype);
   };
}

#endif /* end of include guard: SCRIPTWRAPPER_H */
