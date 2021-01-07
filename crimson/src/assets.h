#pragma once

#include <string>
#include <map>

namespace Crimson {
	class AssetManager {
	private:
		static AssetManager& instance() {
			static AssetManager i;
			return i;
		}

		/* Cache for null-terminated strings, in the format:
		 * { "file-path" : { "file-contents", mod-time }} */
		std::map<std::string, std::pair<std::string, uint64_t>> m_terminatedStrings;
	public:
		static void Init(const char* resDir);
		static void Quit();

		/* Load a null-terminated string from disk */
		static std::string LoadTerminatedString(const char* path, bool reload=false);
	
		/* Iterate currently loaded files, checking for changes.
		 * If there are changes found, reload the file. */
		static void HotReload();
	};
}
