#include "logger.h"
#include "assets.h"
#include "utils/stb_image.h"

#include <physfs.h>

namespace Crimson {
	void AssetManager::Init(const char* resDir) {
		if (PHYSFS_isInit()) { /* Incase we need to re-init */
			PHYSFS_deinit();
		}

		PHYSFS_init(NULL);
		PHYSFS_mount(resDir, "/", 1);
		PHYSFS_setWriteDir(resDir);
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

	unsigned char* AssetManager::LoadBinary(const char* path, bool reload) {
		AssetManager& i = instance();

		/* Check if we cannot use a cached file */
		if (i.m_binary.count(path) == 0 || reload) {
			/* Make sure to free any existing data that might be cached */
			if (reload || i.m_binary[path].first != NULL) {
				free(i.m_binary[path].first);
			}

			PHYSFS_file* file = PHYSFS_openRead(path);
			if (file == NULL) {
				Log(LogType::ERROR, "Failed to load: %s", path);
				return NULL;
			}

			size_t fileSize = PHYSFS_fileLength(file);

			unsigned char* buffer = (unsigned char*)malloc(fileSize + 1);

			PHYSFS_readBytes(file, buffer, fileSize);

			/* To get the modtime */
			PHYSFS_Stat stat;
			PHYSFS_stat(path, &stat);

			i.m_binary[path] = {buffer, stat.modtime};
			
			/* Cleanup */
			PHYSFS_close(file);
		}

		/* Return the cached version of the file */
		return i.m_binary[path].first;
	}

	ref<Shader>& AssetManager::LoadShader(const char* path, bool reload) {
		AssetManager& i = instance();

		/* Check if a cached version cannot be used */
		if (i.m_shaders.count(path) == 0 || reload) {
			/* Load the shader text */
			std::string source = LoadTerminatedString(path, reload);

			/* Parse the shader */
			Shader::ShaderSource parsedSource = Shader::Parse(source.c_str());

			/* To get the modtime */
			PHYSFS_Stat stat;
			PHYSFS_stat(path, &stat);

			i.m_shaders[path] = {ref<Shader>(new Shader(
						parsedSource.vertex.c_str(),
						parsedSource.pixel.c_str())),
						stat.modtime};
		}

		return i.m_shaders[path].first;
	}

	ref<Texture>& AssetManager::LoadTexture(const char* path, bool reload) {
		AssetManager& i = instance();

		if (i.m_textures.count(path) == 0 || reload) {
			/* Load the compressed binary data */
			unsigned char* rawData = LoadBinary(path, reload);

			unsigned char* pixels;
			int w, h, componentCount;
	
			/* To get the modtime & file size */
			PHYSFS_Stat stat;
			PHYSFS_stat(path, &stat);

			/* Uncompress and parse the data */
			pixels = stbi_load_from_memory(rawData, stat.filesize, &w, &h,
					&componentCount, 0);
			
			if (pixels == NULL) {
				Log(LogType::ERROR, "Failed to load %s: %s",
						path, stbi_failure_reason());
			}

			i.m_textures[path] = {ref<Texture>(new Texture(
						pixels, w, h, componentCount)),
						stat.modtime};
		}

		return i.m_textures[path].first;
	}


	void AssetManager::WriteTerminatedString(const char* path, const std::string& text) {
		PHYSFS_File* file = PHYSFS_openWrite(path);

		if (file) {
			PHYSFS_writeBytes(file, text.c_str(), text.size());
			PHYSFS_close(file);
		} else {
			Log(LogType::ERROR, "Failed to write %s, %s", path, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		}
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

		/* Loop over cached binary files */
		for (auto& f : i.m_binary) {
			/* Grab the file stats */
			PHYSFS_Stat stat;
			PHYSFS_stat(f.first.c_str(), &stat);

			/* If there are new changes */
			if (stat.modtime > f.second.second) {
				/* Reload the file */
				LoadBinary(f.first.c_str(), true);
			}
		}

		/* Loop over cached shaders */
		for (auto& f : i.m_shaders) {
			/* Grab the file stats */
			PHYSFS_Stat stat;
			PHYSFS_stat(f.first.c_str(), &stat);

			/* If there are new changes */
			if (stat.modtime > f.second.second) {
				/* Reload the file */
				LoadShader(f.first.c_str(), true);
			}
		}

		/* Loop over cached textures */
		for (auto& f : i.m_textures) {
			/* Grab the file stats */
			PHYSFS_Stat stat;
			PHYSFS_stat(f.first.c_str(), &stat);

			/* If there are new changes */
			if (stat.modtime > f.second.second) {
				/* Reload the file */
				LoadTexture(f.first.c_str(), true);
			}
		}

	}

	bool AssetManager::TextFileModified(const char* path) {
		auto f = instance().m_terminatedStrings[path];

		PHYSFS_Stat stat;
		PHYSFS_stat(path, &stat);

		/* If there are new changes */
		if (stat.modtime > f.second) {
			return true;
		}

		return false;
	}

	
	static std::string GetExtension(const std::string& fname) {
		auto idx = fname.rfind('.');
		std::string extension;

		if(idx != std::string::npos) {
			extension = fname.substr(idx+1);
		}

		return extension;
	}

	std::vector<std::pair<std::string, std::string>> AssetManager::GetDir(const std::string& dir) {
		std::vector<std::pair<std::string, std::string>> result;

		char **rc = PHYSFS_enumerateFiles(dir.c_str());
		char **i;
		for (i = rc; *i != NULL; i++) {
			PHYSFS_Stat stat;
			std::string fname = dir + "/" + *i;
			PHYSFS_stat(fname.c_str(), &stat);

			std::pair<std::string, std::string> currentEntry;

			if (stat.filetype == PHYSFS_FILETYPE_DIRECTORY) {
				auto a = result;
				auto b = GetDir(fname);
				a.insert(a.end(), b.begin(), b.end());
				result = a;
			} else {
				currentEntry.first = fname;
				currentEntry.second = GetExtension(fname);
				result.push_back(currentEntry);
			}

		}
		PHYSFS_freeList(rc);

		return result;
	}

	void AssetManager::Quit() {
		AssetManager& i = instance();

		/* Loop over cached binary files */
		for (auto& f : i.m_binary) {
			free(f.second.first);
		}
		PHYSFS_deinit();
	}
}
