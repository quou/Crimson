#include <assert.h>

#include <tinyxml2.h>

#include "scene.h"
#include "assets.h"
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
					std::string meshName = "mesh" + std::to_string(i);
					printer.OpenElement(meshName.c_str());
						ref<Material> material = mesh->GetMaterial();

						if (material->m_type == "phong") {
							printer.OpenElement("phongmaterial");
								PhongMaterial* phongMat = (PhongMaterial*)material.get();

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
		assert(false && "Function not implemented");
	}
}