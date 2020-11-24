#pragma once

#include "Scene.h"

#include "Utils/tinyxml2.h"

namespace Crimson {
	class Entity;

	class SceneSerialiser {
	private:
		Scene& m_scene;

		Entity ParseEntity(tinyxml2::XMLElement* node);
	public:
		SceneSerialiser(Scene& scene);

		std::string SerialiseString();

		void SerialiseText(const std::string& filePath);
		void SerialiseBinary(const std::string& filePath);

		bool DeserialiseText(const std::string& text);
		bool DeserialiseBinary(const std::string& text);
	};
}
