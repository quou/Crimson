#include <assert.h>

#include <tinyxml2.h>

#include "scene.h"
#include "assets.h"
#include "rendering/meshfactory.h"
#include "rendering/phongmaterial.h"
#include "components/transform.h"
#include "components/script.h"
#include "components/lights.h"
#include "components/renderable.h"

using namespace tinyxml2;

namespace Crimson {
	template <typename T, typename F>
	static void SerialiseComponent(const std::string& componentName, const ref<Entity>& entity, XMLPrinter& printer, F function) {
		if (!entity->HasComponent<T>()) { return; }

		printer.OpenElement(componentName.c_str());

		function(entity->GetComponent<T>(), printer);

		printer.CloseElement();
	}

	static void SerialiseVec3(const char* name, const vec3& val, XMLPrinter& printer) {
		printer.OpenElement(name);
			printer.PushAttribute("x", val.x);
			printer.PushAttribute("y", val.y);
			printer.PushAttribute("z", val.z);
		printer.CloseElement();
	}

	static void SerialiseFloat(const char* name, float val, XMLPrinter& printer) {
		printer.OpenElement(name);
			printer.PushAttribute("v", val);
		printer.CloseElement();
	}

	static void SerialiseString(const char* name, const char* val, XMLPrinter& printer) {
		printer.OpenElement(name);
			printer.PushAttribute("v", val);
		printer.CloseElement();
	}

	static vec3 DeserialiseVec3(XMLElement* node, const char* name) {
		vec3 result;

		XMLElement* el = node->FirstChildElement(name);
		if (el) {
			result.x = el->FloatAttribute("x");
			result.y = el->FloatAttribute("y");
			result.z = el->FloatAttribute("z");
		}

		return result;
	}

	static std::string DeserialiseString(XMLElement* node, const char* name) {
		std::string result;
		
		XMLElement* el = node->FirstChildElement(name);
		if (el) {
			result = el->Attribute("v");
		}

		return result;
	}

	static float DeserialiseFloat(XMLElement* node, const char* name) {
		float result = 0.0f;
		
		XMLElement* el = node->FirstChildElement(name);
		if (el) {
			result = el->FloatAttribute("v");
		}

		return result;
	}

	void SceneSerialiser::SerialiseEntity(const ref<Entity>& entity, XMLPrinter& printer) {
		printer.OpenElement("entity");
			printer.PushAttribute("name", entity->m_name.c_str());

			SerialiseComponent<TransformComponent>("transform", entity, printer, [](void* component, XMLPrinter& printer){
				TransformComponent* tc = (TransformComponent*)component;
				SerialiseVec3("translation", tc->translation, printer);
				SerialiseVec3("rotation", tc->rotation, printer);
				SerialiseVec3("scale", tc->scale, printer);
			});

			SerialiseComponent<ScriptComponent>("script", entity, printer, [](void* component, XMLPrinter& printer){
				ScriptComponent* sc = (ScriptComponent*)component;
				SerialiseString("class", sc->m_behaviourDecl.c_str(), printer);
			});

			SerialiseComponent<PointLightComponent>("pointlight", entity, printer, [](void* component, XMLPrinter& printer){
				PointLightComponent* plc = (PointLightComponent*)component;
				SerialiseVec3("color", plc->color, printer);
				SerialiseFloat("intensity", plc->intensity, printer);
				SerialiseFloat("constant", plc->constant, printer);
				SerialiseFloat("linear", plc->linear, printer);
				SerialiseFloat("quadratic", plc->quadratic, printer);
			});

			SerialiseComponent<SkyLightComponent>("skylight", entity, printer, [](void* component, XMLPrinter& printer){
				SkyLightComponent* slc = (SkyLightComponent*)component;
				SerialiseVec3("color", slc->color, printer);
				SerialiseFloat("intensity", slc->intensity, printer);
			});

			SerialiseComponent<RenderableComponent>("renderable", entity, printer, [](void* component, XMLPrinter& printer){
				RenderableComponent* rc = (RenderableComponent*)component;

				ref<Model> m = rc->m_model;

				int i = 0;
				for (ref<Mesh>& mesh : m->GetMeshList()) {
					printer.OpenElement("mesh");
						printer.PushAttribute("index", i);
						printer.OpenElement("source");
							/* Determine the type of mesh */
							if (mesh->GetInstanceType() == Mesh::INSTANCE) {
								printer.OpenElement("instance");
									Mesh::Type ft = mesh->GetFactoryType();
									printer.PushAttribute("type", ft == Mesh::CUBE ? "cube" : "sphere");
								printer.CloseElement();
							} else if (mesh->GetInstanceType() == Mesh::CUSTOM) {
								assert(false && "Cannot serialise custom meshes yet");
							}
						printer.CloseElement();

						ref<Material> material = mesh->GetMaterial();

						if (material->m_type == "phong") {
							printer.OpenElement("phongmaterial");
								PhongMaterial* phongMat = (PhongMaterial*)material.get();

								SerialiseString("shader", phongMat->m_shader.c_str(), printer);
								SerialiseVec3("color", phongMat->color, printer);
								SerialiseFloat("shininess", phongMat->shininess, printer);
							printer.CloseElement();
						}

					printer.CloseElement();

					i++;
				}
			});

		printer.CloseElement();
	}

	void SceneSerialiser::SerialiseScene(const char* path) {
		XMLPrinter printer;

		printer.OpenElement("scene");

		for (const ref<Entity>& entity : m_scene->m_entities) {
			SerialiseEntity(entity, printer);
		}

		printer.CloseElement();

		AssetManager::WriteTerminatedString(path, printer.CStr());
	}

	void SceneSerialiser::DeserialiseScene(const char* path) {
		XMLDocument doc;
		std::string src = AssetManager::LoadTerminatedString(path);
		doc.Parse(src.c_str(), src.size());
		if (doc.Error()) { 
			Log(LogType::ERROR, "Error parsing scene: %s", doc.ErrorStr());
			return;
		}

		XMLElement* sceneNode = doc.RootElement();
		if (sceneNode) {
			XMLElement* entityNode = sceneNode->FirstChildElement("entity");
			while (entityNode) {
				Entity* newEntity = m_scene->CreateEntity(entityNode->Attribute("name"));

				/* Deserialise transform */
				XMLElement* componentNode = entityNode->FirstChildElement("transform");
				if (componentNode) {
					TransformComponent* tc = newEntity->AddComponent<TransformComponent>();

					tc->translation = DeserialiseVec3(componentNode, "translation");
					tc->rotation = DeserialiseVec3(componentNode, "rotation");
					tc->scale = DeserialiseVec3(componentNode, "scale");
				}

				/* Deserialise renderable */
				componentNode = entityNode->FirstChildElement("renderable");
				if (componentNode) {
					ref<Model> model(new Model());
					XMLElement* meshNode = componentNode->FirstChildElement("mesh");
					while (meshNode) {
						ref<Material> material;

						XMLElement* materialNode = meshNode->FirstChildElement("phongmaterial");
						if (materialNode) {
							material = ref<Material>(new PhongMaterial(
								DeserialiseString(materialNode, "shader").c_str(),
								DeserialiseVec3(materialNode, "color"),
								DeserialiseFloat(materialNode, "shininess")
							));
						} else {
							Log(LogType::WARNING, "No material");
						}

						XMLElement* sourceNode = meshNode->FirstChildElement("source");
						if (sourceNode) {
							XMLElement* instanceNode = sourceNode->FirstChildElement("instance");
							if (instanceNode) {
								std::string instanceType = instanceNode->Attribute("type");
								if (instanceType == "sphere") {
									model->AddMesh(MeshFactory::NewSphereMesh(material));
								} else if (instanceType == "cube") {
									model->AddMesh(MeshFactory::NewCubeMesh(material));
								} else {
									Log(LogType::ERROR, "Unknown mesh instance type: %s", instanceType.c_str());
								}
							}
						} else {
							Log(LogType::WARNING, "No mesh source for renderable component");
						}

						meshNode = meshNode->NextSiblingElement("mesh");
					}

					newEntity->AddComponent<RenderableComponent>(model);
				}

				/* Deserialise point light */
				componentNode = entityNode->FirstChildElement("pointlight");
				if (componentNode) {
					PointLightComponent* plc = newEntity->AddComponent<PointLightComponent>(
						DeserialiseVec3(componentNode, "color"),
						DeserialiseFloat(componentNode, "intensity")
					);

					plc->constant = DeserialiseFloat(componentNode, "constant");
					plc->linear = DeserialiseFloat(componentNode, "linear");
					plc->quadratic = DeserialiseFloat(componentNode, "quadratic");
				}

				/* Deserialise sky light */
				componentNode = entityNode->FirstChildElement("skylight");
				if (componentNode) {
					newEntity->AddComponent<SkyLightComponent>(vec3(1.0f), 0.1f);
				}

				/* Deserialise script */
				componentNode = entityNode->FirstChildElement("script");
				if (componentNode) {
					newEntity->AddComponent<ScriptComponent>(DeserialiseString(componentNode, "class").c_str());
				}

				entityNode = entityNode->NextSiblingElement("entity");
			}
		}
	}
}