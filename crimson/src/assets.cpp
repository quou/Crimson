#include "logger.h"
#include "assets.h"

#include <physfs.h>

namespace Crimson {
	void AssetManager::Init(const char* resDir) {
		if (PHYSFS_isInit()) { /* Incase we need to re-init */
			PHYSFS_deinit();
		}

		PHYSFS_init(NULL);
		PHYSFS_mount(resDir, "/", 1);
	}

	std::string AssetManager::LoadTerminatedString(const char* path, bool reload) {
		AssetManager& i = instance();

		/* Check if we cannot use a cached file */
		if (i.m_terminatedStrings.count(path) == 0 || reload) {
			PHYSFS_file* file = PHYSFS_openRead(path);
			if (file == NULL) {
				Log(LogType::ERROR, "Failed to load: %s", path);
				return "";
			}

			size_t fileSize = PHYSFS_fileLength(file);

			char* buffer = (char*)malloc(fileSize + 1);

			size_t bytesRead = PHYSFS_readBytes(file, buffer, fileSize);
			buffer[bytesRead] = '\0'; /* Null-terminator */

			/* To get the modtime */
			PHYSFS_Stat stat;
			PHYSFS_stat(path, &stat);

			i.m_terminatedStrings[path] = {buffer, stat.modtime};
			
			/* Cleanup */
			free(buffer);
			PHYSFS_close(file);
		}

		/* Return the cached version of the file */
		return i.m_terminatedStrings[path].first;
	}

	void AssetManager::HotReload() {
		AssetManager& i = instance();
		
		/* Loop over cached text files */
		for (auto& f : i.m_terminatedStrings) {
			/* Grab the file stats */
			PHYSFS_Stat stat;
			PHYSFS_stat(f.first.c_str(), &stat);

			/* If there are new changes */
			if (stat.modtime > f.second.second) {
				/* Reload the file */
				LoadTerminatedString(f.first.c_str(), true);
			}
		}	
	}

	void AssetManager::Quit() {
		PHYSFS_deinit();
	}
}
