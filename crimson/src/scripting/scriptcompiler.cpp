#include <angelscript/angelscript.h>
#include <scriptbuilder/scriptbuilder.h>

#include "scriptcompiler.h"
#include "scriptregister.h"
#include "assets.h"

namespace Crimson {
	class CBytecodeStream : public asIBinaryStream {
	public:
		CBytecodeStream(const char* path) {
			f = fopen(path, "wb");
		}

		~CBytecodeStream() { if( f ) fclose(f); }

		int Open(const char *filename) {
			if (f) { return -1; }
			if (f == 0){ return -1; }
			return 0;
		}

		int Write(const void *ptr, asUINT size) {
			if (size == 0 || f == 0) { return 0; }
			fwrite(ptr, size, 1, f); 
			return 0;
		}

		int Read (void *ptr, asUINT size) {
			if(size == 0) { return -1; } 
			fread(ptr, size, 1, f); 
			return 0;
		}
	protected:
		FILE *f;
	};

	ScriptCompiler::ScriptCompiler() {
		m_engine = asCreateScriptEngine();
		ScriptRegister::Register(m_engine);
	}

	ScriptCompiler::~ScriptCompiler()  {
		m_engine->ShutDownAndRelease();
	}

	void ScriptCompiler::AddCode(const char* path) {
		m_code.push_back(AssetManager::LoadTerminatedString(path));
	}
	
	void ScriptCompiler::Compile(const char* moduleName, const char* outputPath) {
		CScriptBuilder builder;
		int r = builder.StartNewModule(m_engine, moduleName); 
		if (r < 0) {
			return;
		}

		int i = 0;
		for (auto& script : m_code) {
			r = builder.AddSectionFromMemory(std::to_string(i).c_str(), script.c_str());
			if (r < 0) {
				continue;
			}
			i++;
		}
		
		r = builder.BuildModule();
		if (r < 0) {
			return;
		}
		
		#warning AngelScript compiler does not make use of the asset manager yet. Make sure to use absolute paths

		CBytecodeStream stream(outputPath);

		asIScriptModule* module = m_engine->GetModule(moduleName);
		module->SaveByteCode(&stream);
	}
}