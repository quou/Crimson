#include "AssetManager.h"

#include "Logger.h"

#include <physfs.h>
#include <tuple>
#include <filesystem>

#include "Utils/stb_image.h"


namespace Crimson {
	AssetManager::AssetManager(bool loadFromArchive) : m_loadFromArchive(loadFromArchive) {
		if (loadFromArchive) {
			PHYSFS_init(NULL);
			PHYSFS_mount("Data.pck", "/", 1);
		}

		m_meshes["Cube"] = new Mesh(
			std::vector<Vertex> {
				{glm::vec3(1.000000, 1.000000, -1.000000), glm::vec3(0.000000, 1.000000, 0.000000), glm::vec2(0.625000, 0.500000)},
				{glm::vec3(-1.000000, 1.000000, -1.000000), glm::vec3(0.000000, 1.000000, 0.000000), glm::vec2(0.875000, 0.500000)},
				{glm::vec3(-1.000000, 1.000000, 1.000000), glm::vec3(0.000000, 1.000000, 0.000000), glm::vec2(0.875000, 0.750000)},
				{glm::vec3(1.000000, 1.000000, -1.000000), glm::vec3(0.000000, 1.000000, 0.000000), glm::vec2(0.625000, 0.500000)},
				{glm::vec3(-1.000000, 1.000000, 1.000000), glm::vec3(0.000000, 1.000000, 0.000000), glm::vec2(0.875000, 0.750000)},
				{glm::vec3(1.000000, 1.000000, 1.000000), glm::vec3(0.000000, 1.000000, 0.000000), glm::vec2(0.625000, 0.750000)},
				{glm::vec3(1.000000, -1.000000, 1.000000), glm::vec3(0.000000, 0.000000, 1.000000), glm::vec2(0.375000, 0.750000)},
				{glm::vec3(1.000000, 1.000000, 1.000000), glm::vec3(0.000000, 0.000000, 1.000000), glm::vec2(0.625000, 0.750000)},
				{glm::vec3(-1.000000, 1.000000, 1.000000), glm::vec3(0.000000, 0.000000, 1.000000), glm::vec2(0.625000, 1.000000)},
				{glm::vec3(1.000000, -1.000000, 1.000000), glm::vec3(0.000000, 0.000000, 1.000000), glm::vec2(0.375000, 0.750000)},
				{glm::vec3(-1.000000, 1.000000, 1.000000), glm::vec3(0.000000, 0.000000, 1.000000), glm::vec2(0.625000, 1.000000)},
				{glm::vec3(-1.000000, -1.000000, 1.000000), glm::vec3(0.000000, 0.000000, 1.000000), glm::vec2(0.375000, 1.000000)},
				{glm::vec3(-1.000000, -1.000000, 1.000000), glm::vec3(-1.000000, 0.000000, 0.000000), glm::vec2(0.375000, 0.000000)},
				{glm::vec3(-1.000000, 1.000000, 1.000000), glm::vec3(-1.000000, 0.000000, 0.000000), glm::vec2(0.625000, 0.000000)},
				{glm::vec3(-1.000000, 1.000000, -1.000000), glm::vec3(-1.000000, 0.000000, 0.000000), glm::vec2(0.625000, 0.250000)},
				{glm::vec3(-1.000000, -1.000000, 1.000000), glm::vec3(-1.000000, 0.000000, 0.000000), glm::vec2(0.375000, 0.000000)},
				{glm::vec3(-1.000000, 1.000000, -1.000000), glm::vec3(-1.000000, 0.000000, 0.000000), glm::vec2(0.625000, 0.250000)},
				{glm::vec3(-1.000000, -1.000000, -1.000000), glm::vec3(-1.000000, 0.000000, 0.000000), glm::vec2(0.375000, 0.250000)},
				{glm::vec3(-1.000000, -1.000000, -1.000000), glm::vec3(0.000000, -1.000000, 0.000000), glm::vec2(0.125000, 0.500000)},
				{glm::vec3(1.000000, -1.000000, -1.000000), glm::vec3(0.000000, -1.000000, 0.000000), glm::vec2(0.375000, 0.500000)},
				{glm::vec3(1.000000, -1.000000, 1.000000), glm::vec3(0.000000, -1.000000, 0.000000), glm::vec2(0.375000, 0.750000)},
				{glm::vec3(-1.000000, -1.000000, -1.000000), glm::vec3(0.000000, -1.000000, 0.000000), glm::vec2(0.125000, 0.500000)},
				{glm::vec3(1.000000, -1.000000, 1.000000), glm::vec3(0.000000, -1.000000, 0.000000), glm::vec2(0.375000, 0.750000)},
				{glm::vec3(-1.000000, -1.000000, 1.000000), glm::vec3(0.000000, -1.000000, 0.000000), glm::vec2(0.125000, 0.750000)},
				{glm::vec3(1.000000, -1.000000, -1.000000), glm::vec3(1.000000, 0.000000, 0.000000), glm::vec2(0.375000, 0.500000)},
				{glm::vec3(1.000000, 1.000000, -1.000000), glm::vec3(1.000000, 0.000000, 0.000000), glm::vec2(0.625000, 0.500000)},
				{glm::vec3(1.000000, 1.000000, 1.000000), glm::vec3(1.000000, 0.000000, 0.000000), glm::vec2(0.625000, 0.750000)},
				{glm::vec3(1.000000, -1.000000, -1.000000), glm::vec3(1.000000, 0.000000, 0.000000), glm::vec2(0.375000, 0.500000)},
				{glm::vec3(1.000000, 1.000000, 1.000000), glm::vec3(1.000000, 0.000000, 0.000000), glm::vec2(0.625000, 0.750000)},
				{glm::vec3(1.000000, -1.000000, 1.000000), glm::vec3(1.000000, 0.000000, 0.000000), glm::vec2(0.375000, 0.750000)},
				{glm::vec3(-1.000000, -1.000000, -1.000000), glm::vec3(0.000000, 0.000000, -1.000000), glm::vec2(0.375000, 0.250000)},
				{glm::vec3(-1.000000, 1.000000, -1.000000), glm::vec3(0.000000, 0.000000, -1.000000), glm::vec2(0.625000, 0.250000)},
				{glm::vec3(1.000000, 1.000000, -1.000000), glm::vec3(0.000000, 0.000000, -1.000000), glm::vec2(0.625000, 0.500000)},
				{glm::vec3(-1.000000, -1.000000, -1.000000), glm::vec3(0.000000, 0.000000, -1.000000), glm::vec2(0.375000, 0.250000)},
				{glm::vec3(1.000000, 1.000000, -1.000000), glm::vec3(0.000000, 0.000000, -1.000000), glm::vec2(0.625000, 0.500000)},
				{glm::vec3(1.000000, -1.000000, -1.000000), glm::vec3(0.000000, 0.000000, -1.000000), glm::vec2(0.375000, 0.500000)},
			},
			std::vector<unsigned int> {
				0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
			}
		);

		m_materials["Default"] = new Material(
			R"(
				shader = "Data/Shaders/Standard.glsl"

				albedo = "Data/GridTexture.png"

				material = {
				    color = {1, 1, 1},
					 smoothness = 1,
					 shininess = 20
				}

			)", *this);
	}

	AssetManager::~AssetManager() {
		for (auto& surface : m_textures) {
			stbi_image_free(surface.second.pixels);
		}

		for (auto& m : m_meshes) {
			delete m.second;
		}

		for (auto& m : m_materials) {
			delete m.second;
		}

		if (m_loadFromArchive) {
			PHYSFS_unmount("data.pck");
			PHYSFS_deinit();
		}
	}

	static std::string GetExtension(const std::string& fname) {
		auto idx = fname.rfind('.');
		std::string extension;

		if(idx != std::string::npos) {
			extension = fname.substr(idx+1);
		}

		return "." + extension;
	}

	static std::vector<std::pair<std::string, std::string>> GetDirReleaseMode(const std::string& dir) {
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
				auto b = GetDirReleaseMode(fname);
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

	Surface* AssetManager::LoadSurface(const std::string& filePath, bool reload) {
		if (m_textFiles.count(filePath) == 0 && !reload) {
			unsigned char* imageData;
			size_t fileSize;

			if (m_loadFromArchive) {
				PHYSFS_file* file = PHYSFS_openRead(filePath.c_str());
				if (file == NULL) {
					CR_LOG_ERROR("Failed to load file: %s.\n", filePath.c_str());
					return NULL;
				}

				fileSize = PHYSFS_fileLength(file);

				imageData = (unsigned char*)malloc(fileSize);
				size_t bytesRead = PHYSFS_readBytes(file, imageData, fileSize);

				PHYSFS_close(file);
			} else {
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
			}

			int x,y;
			int n = 4;
			unsigned char* pixels = stbi_load_from_memory(imageData, fileSize, &x, &y, &n, 0);
			if (pixels == NULL) {
				CR_LOG_ERROR("Failed to load file: %s. Reason for failure: %s\n", filePath.c_str(), stbi_failure_reason());
				return NULL;
			}

			m_textures[filePath] = Surface{x,y,n,pixels};
		}
		return &m_textures[filePath];
	}

	std::string AssetManager::LoadText(const std::string& filePath, bool reload) {
		if (m_textFiles.count(filePath) == 0 && !reload) {
			char* buffer;

			if (m_loadFromArchive) {
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
			} else {
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
			}

			m_textFiles[filePath] = buffer;
			free(buffer);
		}
		return m_textFiles[filePath];
	}

	Mesh* AssetManager::LoadMesh(const std::string& filePath) {
		if (m_meshes.count(filePath) == 0) {
			m_meshes[filePath] = new Mesh(LoadText(filePath).c_str());
		}
		return m_meshes[filePath];
	}

	Material* AssetManager::LoadMaterial(const std::string& filePath) {
		if (m_materials.count(filePath) == 0) {
			m_materials[filePath] = new Material(LoadText(filePath).c_str(), *this);
		}
		return m_materials[filePath];
	}

	std::vector<std::pair<std::string, std::string>> AssetManager::GetFilesFromDir(const std::string& dir) {
		std::vector<std::pair<std::string, std::string>> result;

		if (m_loadFromArchive) {
			result = GetDirReleaseMode(dir);
		} else {
			std::string path = dir;
			for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
				if (!entry.is_directory()) {
					result.push_back({entry.path().string(), entry.path().extension().string()});
				}
			}
		}

		return result;
	}

}
