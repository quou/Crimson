#include "SceneSerialiser.h"

#include "Logger.h"
#include "Entity.h"
#include "Components.h"

#include <yaml-cpp/yaml.h>

#include <fstream>

namespace YAML {
	template <>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3) {
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();

			return true;
		}
	};
}

namespace Crimson {
	SceneSerialiser::SceneSerialiser(Scene& scene) : m_scene(scene) {}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	static void SerialiseEntity(YAML::Emitter& out, Entity ent) {
		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << "Ent";

		if (ent.HasComponent<TransformComponent>()) {
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			auto& transform = ent.GetComponent<TransformComponent>();
			out << YAML::Key << "Name" << YAML::Value << transform.name;
			out << YAML::Key << "Tag" << YAML::Value << transform.tag;
			out << YAML::Key << "GUID" << YAML::Value << transform.guid;

			out << YAML::Key << "Translation" << YAML::Value << transform.position;
			out << YAML::Key << "Rotation" << YAML::Value << transform.rotation;
			out << YAML::Key << "Scale" << YAML::Value << transform.scale;

			out << YAML::EndMap;
		}

		if (ent.HasComponent<MeshFilterComponent>()) {
			out << YAML::Key << "MeshFilterComponent";
			out << YAML::BeginMap;

			auto& mesh = ent.GetComponent<MeshFilterComponent>();
			out << YAML::Key << "Path" << YAML::Value << mesh.path;

			out << YAML::EndMap;
		}

		if (ent.HasComponent<MaterialComponent>()) {
			out << YAML::Key << "MaterialComponent";
			out << YAML::BeginMap;

			auto& mat = ent.GetComponent<MaterialComponent>();
			out << YAML::Key << "Path" << YAML::Value << mat.path;

			out << YAML::EndMap;
		}

		if (ent.HasComponent<BoxColliderComponent>()) {
			out << YAML::Key << "BoxColliderComponent";
			out << YAML::BeginMap;

			auto& box = ent.GetComponent<BoxColliderComponent>();
			out << YAML::Key << "Extents" << YAML::Value << box.extents;

			out << YAML::EndMap;
		}

		if (ent.HasComponent<SphereColliderComponent>()) {
			out << YAML::Key << "SphereColliderComponent";
			out << YAML::BeginMap;

			auto& sphere = ent.GetComponent<SphereColliderComponent>();
			out << YAML::Key << "Radius" << YAML::Value << sphere.radius;

			out << YAML::EndMap;
		}

		if (ent.HasComponent<PhysicsComponent>()) {
			out << YAML::Key << "PhysicsComponent";
			out << YAML::BeginMap;

			auto& phys = ent.GetComponent<PhysicsComponent>();
			out << YAML::Key << "UseGravity" << YAML::Value << phys.useGravity;
			out << YAML::Key << "Mass" << YAML::Value << phys.mass;
			out << YAML::Key << "Friction" << YAML::Value << phys.friction;
			out << YAML::Key << "Bounciness" << YAML::Value << phys.bounciness;
			out << YAML::Key << "IsKinematic" << YAML::Value << phys.isKinematic;
			out << YAML::Key << "CenterOfGravity" << YAML::Value << phys.cog;

			out << YAML::EndMap;
		}

		if (ent.HasComponent<ScriptComponent>()) {
			out << YAML::Key << "ScriptComponent";
			out << YAML::BeginMap;

			auto& script = ent.GetComponent<ScriptComponent>();
			out << YAML::Key << "Class" << YAML::Value << script.className;

			out << YAML::EndMap;
		}

		if (ent.HasComponent<AmbientLightComponent>()) {
			out << YAML::Key << "AmbientLightComponent";
			out << YAML::BeginMap;

			auto& light = ent.GetComponent<AmbientLightComponent>();
			out << YAML::Key << "Color" << YAML::Value << light.color;
			out << YAML::Key << "Intensity" << YAML::Value << light.intensity;

			out << YAML::EndMap;
		}

		if (ent.HasComponent<DirectionalLightComponent>()) {
			out << YAML::Key << "DirectionalLightComponent";
			out << YAML::BeginMap;

			auto& light = ent.GetComponent<DirectionalLightComponent>();
			out << YAML::Key << "Color" << YAML::Value << light.color;
			out << YAML::Key << "Intensity" << YAML::Value << light.intensity;

			out << YAML::EndMap;
		}

		if (ent.HasComponent<PointLightComponent>()) {
			out << YAML::Key << "PointLightComponent";
			out << YAML::BeginMap;

			auto& light = ent.GetComponent<PointLightComponent>();
			out << YAML::Key << "Constant" << YAML::Value << light.constant;
			out << YAML::Key << "Linear" << YAML::Value << light.linear;
			out << YAML::Key << "Quadratic" << YAML::Value << light.quadratic;
			out << YAML::Key << "Color" << YAML::Value << light.color;
			out << YAML::Key << "Intensity" << YAML::Value << light.intensity;

			out << YAML::EndMap;
		}

		if (ent.HasComponent<CameraComponent>()) {
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto& c = ent.GetComponent<CameraComponent>();
			out << YAML::Key << "Active" << YAML::Value << c.active;
			out << YAML::Key << "Near" << YAML::Value << c.camera.m_near;
			out << YAML::Key << "Far" << YAML::Value << c.camera.m_far;
			out << YAML::Key << "FOV" << YAML::Value << c.camera.m_fov;

			out << YAML::EndMap;
		}

		out << YAML::EndMap; // Entity;
	}

	std::string SceneSerialiser::SerialiseString() {
		YAML::Emitter out;

		out << YAML::Comment("Crimson Scene file");

		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << m_scene.m_config.name;
		out << YAML::Key << "Entities" << YAML::BeginSeq;
		m_scene.m_registry.each([&](auto entHandle){
			Entity ent(entHandle, &m_scene);
			if (!ent) {return;}

			SerialiseEntity(out, ent);
		});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		return out.c_str();
	}

	void SceneSerialiser::SerialiseText(const std::string& filePath) {
		std::ofstream fout(filePath);
		fout << SerialiseString();
	}

	void SceneSerialiser::SerialiseBinary(const std::string& filePath) {
		CR_LOG_ERROR("Function %s not implemented", __FUNCTION__);
	}

	bool SceneSerialiser::DeserialiseText(const std::string& text) {
		YAML::Node data = YAML::Load(text);
		if (!data) {
			CR_LOG_ERROR("%s", "Failed to load scene. Check that the file exists and that the YAML is correct.");
			return false;
		}

		std::string sceneName = data["Scene"].as<std::string>();
		m_scene.GetConfig().name = sceneName;

		auto entitiesNode = data["Entities"];
		if (entitiesNode) {
			for (auto ent : entitiesNode) {
				std::string name, tag, guid;
				glm::vec3 pos, rot, sca;
				auto transformComponent = ent["TransformComponent"];
				if (transformComponent) {
					name = transformComponent["Name"].as<std::string>();
					tag = transformComponent["Tag"].as<std::string>();
					guid = transformComponent["GUID"].as<std::string>();
					pos = transformComponent["Translation"].as<glm::vec3>();
					rot = transformComponent["Rotation"].as<glm::vec3>();
					sca = transformComponent["Scale"].as<glm::vec3>();
				}

				auto newEnt = m_scene.CreateEntity(name, tag, guid);
				newEnt.GetComponent<TransformComponent>().position = pos;
				newEnt.GetComponent<TransformComponent>().rotation = rot;
				newEnt.GetComponent<TransformComponent>().scale = sca;

				auto mesh = ent["MeshFilterComponent"];
				if (mesh) {
					auto path = mesh["Path"].as<std::string>();
					newEnt.AddComponent<MeshFilterComponent>(path);
				}

				auto mat = ent["MaterialComponent"];
				if (mat) {
					auto path = mat["Path"].as<std::string>();
					newEnt.AddComponent<MaterialComponent>(path);
				}

				auto script = ent["ScriptComponent"];
				if (script) {
					auto className = script["Class"].as<std::string>();
					newEnt.AddComponent<ScriptComponent>(className);
				}

				auto box = ent["BoxColliderComponent"];
				if (box) {
					auto extents = box["Extents"].as<glm::vec3>();
					newEnt.AddComponent<BoxColliderComponent>(extents);
				}

				auto physics = ent["PhysicsComponent"];
				if (physics) {
					auto useGravity = physics["UseGravity"].as<bool>();
					auto mass = physics["Mass"].as<float>();
					auto friction = physics["Friction"].as<float>();
					auto bounciness = physics["Bounciness"].as<float>();
					auto isKinematic = physics["IsKinematic"].as<bool>();
					auto cog = physics["CenterOfGravity"].as<glm::vec3>();

					newEnt.AddComponent<PhysicsComponent>(useGravity, mass, friction, bounciness, isKinematic, cog);
				}

				auto dirLight = ent["DirectionalLightComponent"];
				if (dirLight) {
					auto color = dirLight["Color"].as<glm::vec3>();
					auto intensity = dirLight["Intensity"].as<float>();

					newEnt.AddComponent<DirectionalLightComponent>(color, intensity);
				}

				auto ambLight = ent["AmbientLightComponent"];
				if (ambLight) {
					auto color = ambLight["Color"].as<glm::vec3>();
					auto intensity = ambLight["Intensity"].as<float>();

					newEnt.AddComponent<AmbientLightComponent>(color, intensity);
				}

				auto poiLight = ent["PointLightComponent"];
				if (poiLight) {
					auto constant = poiLight["Constant"].as<float>();
					auto linear = poiLight["Linear"].as<float>();
					auto quadratic = poiLight["Quadratic"].as<float>();

					auto color = poiLight["Color"].as<glm::vec3>();
					auto intensity = poiLight["Intensity"].as<float>();

					newEnt.AddComponent<PointLightComponent>(constant, linear, quadratic, color, intensity);
				}

				auto cam = ent["CameraComponent"];
				if (cam) {
					auto active = cam["Active"].as<bool>();
					auto near = cam["Near"].as<float>();
					auto far = cam["Far"].as<float>();
					auto fov = cam["FOV"].as<float>();

					newEnt.AddComponent<CameraComponent>(std::pair<int, int>{1366, 768}, fov, near, far, active);
				}
			}
		}

		return true;
	}

	bool SceneSerialiser::DeserialiseBinary(const std::string& text) {
		CR_LOG_ERROR("Function %s not implemented", __FUNCTION__);
		return false;
	}
}
