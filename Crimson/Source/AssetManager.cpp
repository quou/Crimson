#include "AssetManager.h"

#include "Logger.h"

#include <physfs.h>

#include "stb_image.h"

namespace Crimson {
	AssetManager::AssetManager() {
	#ifdef RELEASE
		PHYSFS_init(NULL);
		PHYSFS_mount("data.pck", "/", 1);
	#endif
	}

	AssetManager::~AssetManager() {
		for (auto& surface : m_textures) {
			stbi_image_free(surface.second.pixels);
		}

	#ifdef RELEASE
		PHYSFS_unmount("data.pck");
		PHYSFS_deinit();
	#endif
	}

	Surface* AssetManager::LoadSurface(const std::string& filePath) {
		if (m_textFiles.count(filePath) == 0) {
			unsigned char* imageData;
			size_t fileSize;

	#ifdef RELEASE
			PHYSFS_file* file = PHYSFS_openRead(filePath.c_str());
			if (file == NULL) {
				CR_LOG_ERROR("Failed to load file: %s.\n", filePath.c_str());
				return NULL;
			}

			fileSize = PHYSFS_fileLength(file);

			imageData = (char*)malloc(fileSize);
			size_t bytesRead = PHYSFS_readBytes(file, imageData, fileSize);

			imageData[bytesRead];

			PHYSFS_close(file);
	#else
			FILE* file = fopen(filePath.c_str(), "rb");
			if (file == NULL) {
				CR_LOG_ERROR("Failed to load file: %s.\n", filePath.c_str());
				return NULL;
			}

			fseek(file, 0L, SEEK_END);
			fileSize = ftell(file);
			rewind(file);

			imageData = (unsigned char*)malloc(fileSize);

			size_t bytesRead = fread(imageData, sizeof(char), fileSize, file);

			fclose(file);
	#endif

			int x,y;
			int n = 4;
			unsigned char* pixels = stbi_load_from_memory(imageData, fileSize, &x, &y, &n, 4);
			if (pixels == NULL) {
				CR_LOG_ERROR("Failed to load file: %s. Reason for failure: %s\n", filePath.c_str(), stbi_failure_reason());
				return NULL;
			}

			m_textures[filePath] = Surface{x,y,n,pixels};
		}
		return &m_textures[filePath];
	}

	std::string AssetManager::LoadText(const std::string& filePath) {
		if (m_textFiles.count(filePath) == 0) {
			char* buffer;

	#ifdef RELEASE
			PHYSFS_file* file = PHYSFS_openRead(filePath.c_str());
			if (file == NULL) {
				CR_LOG_ERROR("Failed to load file: %s.\n", filePath.c_str());
				return "";
			}

			size_t fileSize = PHYSFS_fileLength(file);

			buffer = (char*)malloc(fileSize + 1);
			size_t bytesRead = PHYSFS_readBytes(file, buffer, fileSize);

			buffer[bytesRead] = '\0';

			PHYSFS_close(file);
	#else
			FILE* file = fopen(filePath.c_str(), "r");
			if (file == NULL) {
				CR_LOG_ERROR("Failed to load file: %s.\n", filePath.c_str());
				return "";
			}

			fseek(file, 0L, SEEK_END);
			size_t fileSize = ftell(file);
			rewind(file);

			buffer = (char*)malloc(fileSize + 1);

			size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);

			buffer[bytesRead] = '\0';

			fclose(file);
	#endif

			m_textFiles[filePath] = buffer;
			free(buffer);
		}
		return m_textFiles[filePath];
	}

}
