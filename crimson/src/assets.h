#pragma once

#include <string>
#include <map>

#include "core.h"
#include "rendering/shader.h"
#include "rendering/model.h"
#include "memory.h"

namespace Crimson {
	/* Handles loading, caching and saving of files from disk */
	class CR_API AssetManager {
	private:
		static AssetManager& instance() {
			static AssetManager i;
			return i;
		}

		/* Cache for null-terminated strings, in the format:
		 * { "file-path" : { "file-contents", mod-time }} */
		std::map<std::string, std::pair<std::string, uint64_t>> m_terminatedStrings;


		/* Cache for binary, in the format:
		 * { "file-path" : { unsigned char* file-contents, mod-time }} */
		std::map<std::string, std::pair<unsigned char*, uint64_t>> m_binary;
	
		/* Cache for shaders, in the format:
		 * { "file-path" : { ref<Shader>, mod-time }} */
		std::map<std::string, std::pair<ref<Shader>, uint64_t>> m_shaders;
	public:
		static void Init(const char* resDir);
		static void Quit();

		/* Load a null-terminated string from disk */
		static std::string LoadTerminatedString(const char* path, bool reload=false);

		/* Load the raw bytes from disk */
		static unsigned char* LoadBinary(const char* path, bool reload=false);

		/* Load, parse and compile a  shader from a text file */
		static ref<Shader>& LoadShader(const char* path, bool reload = false);

		inline static const std::map<std::string, std::pair<ref<Shader>, uint64_t>>& GetShaders() {
			return instance().m_shaders;
		}

		/* Iterate currently loaded files, checking for changes.
		 * If there are changes found, reload the file. */
		static void HotReload();
	};
}
