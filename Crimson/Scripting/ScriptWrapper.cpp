#include "ScriptWrapper.h"

#include "ScriptStandardFunctions.h"

#include "ComponentSystems/CameraSystems.h"
#include "ComponentSystems/Transform.h"
#include "ComponentSystems/GraphicsSystems.h"
#include "chaiscript/chaiscript.hpp"


#include <glm/glm.hpp>

namespace Crimson {
   namespace Scripting {
      void Execute(ECS& ecs, EntityHandle ent, ScriptComponent* scriptComponent) {
         chaiscript::ChaiScript* chai = &scriptComponent->chai;

         chai->add(chaiscript::user_type<glm::vec3>(), "vec3");
         chai->add(chaiscript::constructor<glm::vec3()>(), "vec3");
         chai->add(chaiscript::constructor<glm::vec3(float)>(), "vec3");
         chai->add(chaiscript::constructor<glm::vec3(float, float, float)>(), "vec3");
         chai->add(chaiscript::fun(&glm::vec3::x), "x");
         chai->add(chaiscript::fun(&glm::vec3::y), "y");
         chai->add(chaiscript::fun(&glm::vec3::z), "z");

         chai->add(chaiscript::user_type<Material>(), "Material");
         chai->add(chaiscript::fun(&Material::ambient), "ambient");
         chai->add(chaiscript::fun(&Material::diffuse), "diffuse");
         chai->add(chaiscript::fun(&Material::specular), "specular");
         chai->add(chaiscript::fun(&Material::shininess), "shininess");

         chai->add(chaiscript::user_type<Model>(), "Mesh");
         chai->add(chaiscript::fun(&Model::Load), "Load");
         chai->add(chaiscript::fun(&Model::GetRes), "GetRes");

         chai->add(chaiscript::user_type<Texture>(), "Texture");
         chai->add(chaiscript::fun(&Texture::Load), "Load");
         chai->add(chaiscript::fun(&Texture::GetRes), "GetRes");
         chai->add(chaiscript::fun(&Texture::IsLoaded), "IsLoaded");

         chai->add(chaiscript::user_type<Shader>(), "Shader");
         chai->add(chaiscript::fun(&Shader::Init), "Init");
         chai->add(chaiscript::fun(&Shader::GetVertPath), "GetVertPath");
         chai->add(chaiscript::fun(&Shader::GetFragPath), "GetFragPath");

         chai->add(chaiscript::user_type<Camera>(), "Camera");
         chai->add(chaiscript::fun(&Camera::SetFOV), "SetFOV");
         chai->add(chaiscript::fun(&Camera::SetNear), "SetNear");
         chai->add(chaiscript::fun(&Camera::SetFar), "SetFar");
         chai->add(chaiscript::fun(&Camera::GetFOV), "GetFOV");
         chai->add(chaiscript::fun(&Camera::GetNear), "GetNear");
         chai->add(chaiscript::fun(&Camera::GetFar), "GetFar");


         chai->add(chaiscript::user_type<Transform>(), "TransformComponent");
         chai->add(chaiscript::fun(&Transform::position), "position");
         chai->add(chaiscript::fun(&Transform::rotation), "rotation");
         chai->add(chaiscript::fun(&Transform::scale), "scale");

         chai->add(chaiscript::user_type<CameraComponent>(), "CameraComponent");
         chai->add(chaiscript::fun(&CameraComponent::isCurrent), "active");
         chai->add(chaiscript::fun(&CameraComponent::camera), "camera");


         chai->add(chaiscript::user_type<ModelComponent>(), "ModelComponent");
         chai->add(chaiscript::fun(&ModelComponent::model), "mesh");
         chai->add(chaiscript::fun(&ModelComponent::material), "material");
         chai->add(chaiscript::fun(&ModelComponent::shader), "shader");
         chai->add(chaiscript::fun(&ModelComponent::texture), "texture");


         chai->add(chaiscript::user_type<PointLight>(), "PointLightComponent");
         chai->add(chaiscript::fun(&PointLight::constant), "constant");
         chai->add(chaiscript::fun(&PointLight::linear), "linear");
         chai->add(chaiscript::fun(&PointLight::quadratic), "quadratic");
         chai->add(chaiscript::fun(&PointLight::ambient), "ambient");
         chai->add(chaiscript::fun(&PointLight::diffuse), "diffuse");
         chai->add(chaiscript::fun(&PointLight::specular), "specular");

         chai->add(chaiscript::user_type<EntityHandle>(), "EntityHandle");
         const EntityHandle e = ent;
         chai->add_global_const(chaiscript::const_var(e), "entity");

         chai->add(chaiscript::fun(&ECS::GetComponent<Transform>, &ecs), "GetTransformComponent");
         chai->add(chaiscript::fun(&ECS::GetComponent<PointLight>, &ecs), "GetPointLightComponent");
         chai->add(chaiscript::fun(&ECS::GetComponent<ModelComponent>, &ecs), "GetModelComponent");

         try {
            chai->use(scriptComponent->scriptFile);
         } catch (const chaiscript::exception::eval_error &e) {
            std::cout << e.pretty_print() << '\n';
         } catch (const double e) {
         } catch (int) {
         } catch (float) {
         } catch (const std::string &) {
         } catch (const std::exception &e) {
           std::cout << e.what() << '\n';
         }
      }

      void CallUpdateFunction(ScriptComponent* scriptComponent, float delta) {
         try {
            auto p = scriptComponent->chai.eval<std::function<void (float)>>("OnUpdate");
            p(delta);
         } catch (const chaiscript::exception::eval_error &e) {
            std::cout << e.pretty_print() << '\n';
         } catch (const double e) {
         } catch (int) {
         } catch (float) {
         } catch (const std::string &) {
         } catch (const std::exception &e) {
           std::cout << e.what() << '\n';
         }
      }

      void CallBeginFunction(ScriptComponent* scriptComponent) {
         try {
            auto p = scriptComponent->chai.eval<std::function<void ()>>("OnBegin");
            p();
         } catch (const chaiscript::exception::eval_error &e) {
            std::cout << e.pretty_print() << '\n';
         } catch (const double e) {
         } catch (int) {
         } catch (float) {
         } catch (const std::string &) {
         } catch (const std::exception &e) {
           std::cout << e.what() << '\n';
         }
      }
   }
}
