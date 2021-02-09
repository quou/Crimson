#include <assert.h>

#include <tinyxml2.h>
#include <angelscript/angelscript.h>

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

	static void SerialiseBool(const char* name, bool val, XMLPrinter& printer) {
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

	static bool DeserialiseBool(XMLElement* node, const char* name) {
		bool result;
		
		XMLElement* el = node->FirstChildElement(name);
		if (el) {
			result = el->BoolAttribute("v");
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

				printer.OpenElement("serialisablefloats");
				for (auto& f : sc->m_serialisableFloats) {
					printer.OpenElement(f.first.name.c_str());
						printer.PushAttribute("v", f.second);
						printer.PushAttribute("i", f.first.index);
					printer.CloseElement();
				}
				printer.CloseElement();

				printer.OpenElement("serialisableints");
				for (auto& f : sc->m_serialisableInts) {
					printer.OpenElement(f.first.name.c_str());
						printer.PushAttribute("v", f.second);
						printer.PushAttribute("i", f.first.index);
					printer.CloseElement();
				}
				printer.CloseElement();

				printer.OpenElement("serialisablestrings");
				for (auto& f : sc->m_serialisableStrings) {
					printer.OpenElement(f.first.name.c_str());
						printer.PushAttribute("v", f.second.c_str());
						printer.PushAttribute("i", f.first.index);
					printer.CloseElement();
				}
				printer.CloseElement();
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

			SerialiseComponent<SunComponent>("sun", entity, printer, [](void* component, XMLPrinter& printer){
				SunComponent* slc = (SunComponent*)component;
				SerialiseVec3("direction", slc->direction, printer);
				SerialiseVec3("color", slc->color, printer);
				SerialiseFloat("intensity", slc->intensity, printer);
				SerialiseBool("castshadows", slc->castShadows, printer);
			});

			SerialiseComponent<CameraComponent>("camera", entity, printer, [](void* component, XMLPrinter& printer){
				CameraComponent* cc = (CameraComponent*)component;
				SerialiseBool("active", cc->active, printer);
			});

			SerialiseComponent<RenderableComponent>("renderable", entity, printer, [](void* component, XMLPrinter& printer){
				RenderableComponent* rc = (RenderableComponent*)component;

				ref<Model> m = rc->m_model;
				if (m->IsFromFile()) {
					SerialiseString("source", m->GetPath().c_str(), printer);
				}

				int i = 0;
				for (ref<Mesh>& mesh : m->GetMeshList()) {
					printer.OpenElement("mesh");
						printer.PushAttribute("index", i);
							if (!m->IsFromFile()) {
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
							}

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

	std::string SceneSerialiser::SerialiseScene(const char* path) {
		XMLPrinter printer;

		printer.OpenElement("scene");

		for (const ref<Entity>& entity : m_scene->m_entities) {
			SerialiseEntity(entity, printer);
		}

		printer.CloseElement();

		/* Only save to a file if the path isn't NULL, 
		 * to allow serialisation to memory only */
		if (path != NULL) { 
			AssetManager::WriteTerminatedString(path, printer.CStr());
		}

		return printer.CStr();
	}

	void SceneSerialiser::DeserialiseScene(const char* path, bool dontInitScripts) {
		std::string src = AssetManager::LoadTerminatedString(path);
		DeserialiseSceneFromMemory(src, dontInitScripts);
	}

	void SceneSerialiser::DeserialiseSceneFromMemory(const std::string& xml, bool dontInitScripts) {
		XMLDocument doc;

		doc.Parse(xml.c_str(), xml.size());
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
					ref<Model> model;
					
					XMLElement* modelSourceNode = componentNode->FirstChildElement("source");
					if (modelSourceNode) {
						model = ref<Model>(new Model(modelSourceNode->Attribute("v")));
					} else {
						model = ref<Model>(new Model());
					}

					XMLElement* meshNode = componentNode->FirstChildElement("mesh");
					while (meshNode) {
						int meshIndex = meshNode->IntAttribute("index");

						ref<Material> material;

						XMLElement* materialNode = meshNode->FirstChildElement("phongmaterial");
						if (materialNode) {
							material = ref<Material>(new PhongMaterial(
								DeserialiseString(materialNode, "shader").c_str(),
								DeserialiseVec3(materialNode, "color"),
								DeserialiseFloat(materialNode, "shininess")
							));

							if (modelSourceNode) {
								std::vector<ref<Mesh>>& meshes = model->GetMeshList();

								if (meshes.size() >= meshIndex && meshIndex >= 0) {
									meshes[meshIndex]->UseMaterial(material);
								}
							}
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
					newEntity->AddComponent<SkyLightComponent>(
						DeserialiseVec3(componentNode, "color"),
						DeserialiseFloat(componentNode, "intensity")
					);
				}

				/* Deserialise sky light */
				componentNode = entityNode->FirstChildElement("sun");
				if (componentNode) {
					newEntity->AddComponent<SunComponent>(
						DeserialiseVec3(componentNode, "direction"),
						DeserialiseVec3(componentNode, "color"),
						DeserialiseFloat(componentNode, "intensity"),
						DeserialiseBool(componentNode, "castshadows")
					);
				}

				/* Deserialise camera */
				componentNode = entityNode->FirstChildElement("camera");
				if (componentNode) {
					newEntity->AddComponent<CameraComponent>(
						DeserialiseBool(componentNode, "active")
					);
				}

				/* Deserialise script */
				componentNode = entityNode->FirstChildElement("script");
				if (componentNode) {
					ScriptComponent* sc = newEntity->AddComponentManualInit<ScriptComponent>(DeserialiseString(componentNode, "class").c_str());

					XMLElement* sfloatNode = componentNode->FirstChildElement("serialisablefloats");
					if (sfloatNode) {
						XMLElement* node = sfloatNode->FirstChildElement();
						while (node) {
							BehaviourField field = BehaviourField{node->Name(), asTYPEID_FLOAT, node->UnsignedAttribute("i")};

							sc->m_serialisableFloats[field] = node->FloatAttribute("v");

							node = node->NextSiblingElement();
						}
					}

					XMLElement* sintNode = componentNode->FirstChildElement("serialisableints");
					if (sintNode) {
						XMLElement* node = sintNode->FirstChildElement();
						while (node) {
							BehaviourField field = BehaviourField{node->Name(), asTYPEID_INT32, node->UnsignedAttribute("i")};

							sc->m_serialisableInts[field] = node->IntAttribute("v");

							node = node->NextSiblingElement();
						}
					}

					XMLElement* sstringNode = componentNode->FirstChildElement("serialisablestrings");
					if (sstringNode) {
						XMLElement* node = sstringNode->FirstChildElement();
						while (node) {
							BehaviourField field = BehaviourField{node->Name(), m_scene->m_scriptManager->GetStringTypeID(), node->UnsignedAttribute("i")};

							sc->m_serialisableStrings[field] = node->Attribute("v");

							node = node->NextSiblingElement();
						}
					}

					sc->m_dontCallInit = dontInitScripts;
					sc->OnInit();
				}

				entityNode = entityNode->NextSiblingElement("entity");
			}
		}
	}
}