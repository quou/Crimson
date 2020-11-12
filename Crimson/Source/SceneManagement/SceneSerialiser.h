#pragma once

#include "Scene.h"

namespace Crimson {
	class SceneSerialiser {
	private:
		Scene& m_scene;
	public:
		SceneSerialiser(Scene& scene);

		std::string SerialiseString();

		void SerialiseText(const std::string& filePath);
		void SerialiseBinary(const std::string& filePath);

		bool DeserialiseText(const std::string& text);
		bool DeserialiseBinary(const std::string& text);
	};
}
