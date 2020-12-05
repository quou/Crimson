#pragma once

#include <map>
#include <string>

#include "Renderer/Surface.h"

#include "Renderer/Renderer3D/Mesh.h"
#include "Renderer/Material.h"

namespace Crimson {
	class AssetManager {
	private:
		std::map<std::string, std::string> m_textFiles;
		std::map<std::string, Surface> m_textures;
		std::map<std::string, Mesh*> m_meshes;
		std::map<std::string, Material*> m_materials;

		bool m_loadFromArchive;

		std::string m_workingDir;
	public:
		AssetManager(bool loadFromArchive, const std::string& workingDir = "");
		Surface* LoadSurface(const std::string& f, bool reload=false);
		std::string LoadText(const std::string& f, bool reload=false, bool useWorkingDir=true);
		Mesh* LoadMesh(const std::string& f);
		Material* LoadMaterial(const std::string& f);
		~AssetManager();

		std::vector<std::pair<std::string, std::string>> GetFilesFromDir(const std::string& dir);

		inline const std::string& GetWorkingDir() {return m_workingDir;}
		inline void SetWorkingDir(const std::string& workingDir) {m_workingDir = workingDir;}
	};
}
