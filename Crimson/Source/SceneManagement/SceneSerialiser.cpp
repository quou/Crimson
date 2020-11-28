#include "SceneSerialiser.h"

#include "Logger.h"
#include "Components.h"

#include <fstream>

using namespace tinyxml2;

namespace Crimson {
	SceneSerialiser::SceneSerialiser(Scene& scene) : m_scene(scene) {}

	static auto FindNode(XMLElement* node, const char* name) {
		std::vector<XMLElement*> foundNodes;

		for (auto element = node->FirstChildElement(name); element; element = element->NextSiblingElement(name)) {
			foundNodes.push_back(element);
		}

		return foundNodes;
	}

	static void SerialiseVec3(XMLPrinter& printer, const char* name, const glm::vec3& vec) {
		printer.OpenElement(name);
		printer.PushAttribute("x", vec.x);
		printer.PushAttribute("y", vec.y);
		printer.PushAttribute("z", vec.z);
		printer.CloseElement();
	}

	static void SerialiseQuat(XMLPrinter& printer, const char* name, const glm::quat& q) {
		printer.OpenElement(name);
		printer.PushAttribute("w", q.w);
		printer.PushAttribute("x", q.x);
		printer.PushAttribute("y", q.y);
		printer.PushAttribute("z", q.z);
		printer.CloseElement();
	}

	template <typename T, typename ComponentDrawFunction>
	static void SerialiseComponent(XMLPrinter& printer, const std::string& componentName, Crimson::Entity ent, ComponentDrawFunction drawFunction) {
		if (ent.HasComponent<T>()) {
			printer.OpenElement(componentName.c_str());
			drawFunction(printer, ent.GetComponent<T>());
			printer.CloseElement();
		}
	}

	static void SerialiseEntity(XMLPrinter& printer, Entity ent) {
		printer.OpenElement("entity");
			auto& transform = ent.GetComponent<TransformComponent>();

			printer.PushAttribute("name", transform.name.c_str());
			printer.PushAttribute("tag", transform.tag.c_str());

			printer.OpenElement("transform");
				SerialiseVec3(printer, "position", transform.position);
				SerialiseQuat(printer, "rotation", transform.rotation);
				SerialiseVec3(printer, "scale", transform.scale);
			printer.CloseElement();

			SerialiseComponent<MeshFilterComponent>(printer, "meshfilter", ent, [](auto& printer, auto& component){
				printer.OpenElement("resource");
					printer.PushAttribute("path", component.path.c_str());
				printer.CloseElement();
			});

			SerialiseComponent<MaterialComponent>(printer, "material", ent, [](auto& printer, auto& component){
				printer.OpenElement("resource");
					printer.PushAttribute("path", component.path.c_str());
				printer.CloseElement();
			});

			SerialiseComponent<CameraComponent>(printer, "camera", ent, [](auto& printer, auto& component){
				printer.PushAttribute("active", component.active);

				printer.OpenElement("perspective");

					printer.PushAttribute("fov", component.camera.m_fov);
					printer.PushAttribute("near", component.camera.m_near);
					printer.PushAttribute("far", component.camera.m_far);

				printer.CloseElement();
			});

			SerialiseComponent<BoxColliderComponent>(printer, "boxcollider", ent, [](auto& printer, auto& component){
				SerialiseVec3(printer, "extents", component.extents);
			});

			SerialiseComponent<SphereColliderComponent>(printer, "spherecollider", ent, [](auto& printer, auto& component){
				printer.PushAttribute("radius", component.radius);
			});

			SerialiseComponent<PhysicsComponent>(printer, "physics", ent, [](auto& printer, auto& component){
				printer.PushAttribute("usegravity", component.useGravity);
				printer.PushAttribute("mass", component.mass);
				printer.PushAttribute("friction", component.friction);
				printer.PushAttribute("bounciness", component.bounciness);
				printer.PushAttribute("iskinematic", component.isKinematic);
				SerialiseVec3(printer, "cog", component.cog);
			});

			SerialiseComponent<ScriptComponent>(printer, "script", ent, [](auto& printer, auto& component){
				printer.PushAttribute("class", component.className.c_str());
			});

			SerialiseComponent<AmbientLightComponent>(printer, "ambientlight", ent, [](auto& printer, auto& component){
				printer.PushAttribute("intensity", component.intensity);

				SerialiseVec3(printer, "color", component.color);
			});

			SerialiseComponent<DirectionalLightComponent>(printer, "directionallight", ent, [](auto& printer, auto& component){
				printer.PushAttribute("intensity", component.intensity);

				SerialiseVec3(printer, "color", component.color);
			});

			SerialiseComponent<PointLightComponent>(printer, "pointlight", ent, [](auto& printer, auto& component){
				printer.PushAttribute("intensity", component.intensity);
				printer.PushAttribute("constant", component.constant);
				printer.PushAttribute("linear", component.linear);
				printer.PushAttribute("quadratic", component.quadratic);

				SerialiseVec3(printer, "color", component.color);
			});

			for (auto child : ent.GetComponent<TransformComponent>().children) {
				SerialiseEntity(printer, child);
			}

		printer.CloseElement();
	}

	std::string SceneSerialiser::SerialiseString() {
		XMLPrinter printer;

      printer.OpenElement("scene");

	      /* SAVE SCENE CONFIG */
	      printer.OpenElement("config");
				printer.PushAttribute("name", m_scene.m_config.name.c_str());
				SerialiseVec3(printer, "clearcolor", m_scene.m_config.clearColor);
				if (m_scene.m_config.useSkybox) {
		         printer.OpenElement("skybox");
						printer.PushAttribute("posx", m_scene.m_config.skyboxPosX.c_str());
						printer.PushAttribute("negx", m_scene.m_config.skyboxNegX.c_str());
						printer.PushAttribute("posy", m_scene.m_config.skyboxPosY.c_str());
						printer.PushAttribute("negy", m_scene.m_config.skyboxNegY.c_str());
						printer.PushAttribute("posz", m_scene.m_config.skyboxPosZ.c_str());
						printer.PushAttribute("negz", m_scene.m_config.skyboxNegZ.c_str());
					printer.CloseElement();
				}
	      printer.CloseElement();

			printer.OpenElement("entities");
				m_scene.m_registry.each([&](auto entHandle){
					Entity ent(entHandle, &m_scene);
					if (!ent) {return;}

					if (!ent.GetComponent<TransformComponent>().parent) {
						SerialiseEntity(printer, ent);
					}
				});
			printer.CloseElement();

		printer.CloseElement();

		return printer.CStr();
	}

	std::string SceneSerialiser::SerialiseText(const std::string& filePath) {
		std::ofstream fout(filePath);
		auto str = SerialiseString();
		fout << str;
		return str;
	}

	void SceneSerialiser::SerialiseBinary(const std::string& filePath) {
		CR_LOG_ERROR("Function %s not implemented", __FUNCTION__);
	}

	static glm::vec3 ParseVec3(XMLElement* rootNode, const char* name) {
		glm::vec3 result;

		auto el = rootNode->FirstChildElement(name);
		if (el) {
			result.x = el->FloatAttribute("x");
			result.y = el->FloatAttribute("y");
			result.z = el->FloatAttribute("z");
		}

		return result;
	}

	static glm::quat ParseQuat(XMLElement* rootNode, const char* name) {
		glm::quat result;

		auto el = rootNode->FirstChildElement(name);
		if (el) {
			result.w = el->FloatAttribute("w");
			result.x = el->FloatAttribute("x");
			result.y = el->FloatAttribute("y");
			result.z = el->FloatAttribute("z");
		}

		return result;
	}

	Entity SceneSerialiser::ParseEntity(XMLElement* node) {
		Entity ent = m_scene.CreateEntity(node->Attribute("name"), node->Attribute("tag"));

		auto eComponent = node->FirstChildElement("transform");
		if (eComponent) {
			auto& transform = ent.GetComponent<TransformComponent>();

			transform.position = ParseVec3(eComponent, "position");
			transform.rotation = ParseQuat(eComponent, "rotation");
			transform.scale = ParseVec3(eComponent, "scale");
		}

		eComponent = node->FirstChildElement("meshfilter");
		if (eComponent) {
			auto resourceNode = eComponent->FirstChildElement("resource");
			if (resourceNode) {
				ent.AddComponent<MeshFilterComponent>().path = resourceNode->Attribute("path");
			}
		}

		eComponent = node->FirstChildElement("material");
		if (eComponent) {
			auto resourceNode = eComponent->FirstChildElement("resource");
			if (resourceNode) {
				ent.AddComponent<MaterialComponent>().path = resourceNode->Attribute("path");
			}
		}

		eComponent = node->FirstChildElement("camera");
		if (eComponent) {
			auto& camera = ent.AddComponent<CameraComponent>();

			camera.active = eComponent->BoolAttribute("active");

			auto perspectiveNode = eComponent->FirstChildElement("perspective");
			if (perspectiveNode) {
				camera.camera.m_fov = perspectiveNode->FloatAttribute("fov");
				camera.camera.m_near = perspectiveNode->FloatAttribute("near");
				camera.camera.m_far = perspectiveNode->FloatAttribute("far");
			}
		}

		eComponent = node->FirstChildElement("boxcollider");
		if (eComponent) {
			auto& box = ent.AddComponent<BoxColliderComponent>();

			box.extents = ParseVec3(eComponent, "extents");
		}

		eComponent = node->FirstChildElement("spherecollider");
		if (eComponent) {
			auto& sphere = ent.AddComponent<SphereColliderComponent>();

			sphere.radius = eComponent->FloatAttribute("radius");
		}

		eComponent = node->FirstChildElement("physics");
		if (eComponent) {
			auto& physics = ent.AddComponent<PhysicsComponent>();

			physics.useGravity = eComponent->BoolAttribute("usegravity");
			physics.mass = eComponent->FloatAttribute("mass");
			physics.friction = eComponent->FloatAttribute("friction");
			physics.bounciness = eComponent->FloatAttribute("bounciness");
			physics.isKinematic = eComponent->BoolAttribute("iskinematic");

			physics.cog = ParseVec3(eComponent, "cog");
		}

		eComponent = node->FirstChildElement("script");
		if (eComponent) {
			auto& script = ent.AddComponent<ScriptComponent>();

			script.className = eComponent->Attribute("class");
		}

		eComponent = node->FirstChildElement("ambientlight");
		if (eComponent) {
			auto& light = ent.AddComponent<AmbientLightComponent>();

			light.intensity = eComponent->FloatAttribute("intensity");
			light.color = ParseVec3(eComponent, "color");
		}

		eComponent = node->FirstChildElement("directionallight");
		if (eComponent) {
			auto& light = ent.AddComponent<DirectionalLightComponent>();

			light.intensity = eComponent->FloatAttribute("intensity");
			light.color = ParseVec3(eComponent, "color");
		}

		eComponent = node->FirstChildElement("pointlight");
		if (eComponent) {
			auto& light = ent.AddComponent<PointLightComponent>();

			light.intensity = eComponent->FloatAttribute("intensity");
			light.constant = eComponent->FloatAttribute("constant");
			light.linear = eComponent->FloatAttribute("linear");
			light.quadratic = eComponent->FloatAttribute("quadratic");
			light.color = ParseVec3(eComponent, "color");
		}

		return ent;
	}

	void SceneSerialiser::ParseEntities(XMLElement* node, Entity parent) {
		for (auto element : FindNode(node, "entity")) {
			auto ent = ParseEntity(element);

			if (parent && ent) {
				ent.GetComponent<TransformComponent>().parent = parent;
				parent.GetComponent<TransformComponent>().children.push_back(ent);
			}

			ParseEntities(element, ent);
		}
	}

	bool SceneSerialiser::DeserialiseText(const std::string& text) {

		XMLDocument doc;
		doc.Parse(text.c_str());

		XMLElement* configNode = doc.RootElement()->FirstChildElement("config");
		if (configNode) {
			m_scene.m_config.name = configNode->Attribute("name");

			m_scene.m_config.clearColor = ParseVec3(configNode, "clearcolor");

			XMLElement* skyboxNode = configNode->FirstChildElement("skybox");
			bool useSkybox = skyboxNode;
			m_scene.m_config.useSkybox = useSkybox;
			if (useSkybox) {
				m_scene.m_config.skyboxPosX = skyboxNode->Attribute("posx");
				m_scene.m_config.skyboxNegX = skyboxNode->Attribute("negx");
				m_scene.m_config.skyboxPosY = skyboxNode->Attribute("posy");
				m_scene.m_config.skyboxNegY = skyboxNode->Attribute("negy");
				m_scene.m_config.skyboxPosZ = skyboxNode->Attribute("posz");
				m_scene.m_config.skyboxNegZ = skyboxNode->Attribute("negz");
				m_scene.LoadSkybox();
			}
		}

		ParseEntities(doc.RootElement()->FirstChildElement("entities"));

		return true;
	}

	bool SceneSerialiser::DeserialiseBinary(const std::string& text) {
		CR_LOG_ERROR("Function %s not implemented", __FUNCTION__);
		return false;
	}
}
