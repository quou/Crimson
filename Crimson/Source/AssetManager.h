#pragma once

#include <map>
#include <string>

#include "Renderer/Surface.h"

namespace Crimson {
	class AssetManager {
	private:
		std::map<std::string, std::string> m_textFiles;
		std::map<std::string, Surface> m_textures;
	public:
		AssetManager();
		Surface* LoadSurface(const std::string& filePath);
		std::string LoadText(const std::string& filePath);
		~AssetManager();
	};
}
