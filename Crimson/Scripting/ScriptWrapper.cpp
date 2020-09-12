#include "ScriptWrapper.h"

#include "ScriptStandardFunctions.h"
#include "ComponentSystems/Transform.h"
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

         chai->add(chaiscript::user_type<Transform>(), "Transform");
         chai->add(chaiscript::fun(&Transform::position), "position");
         chai->add(chaiscript::fun(&Transform::rotation), "rotation");
         chai->add(chaiscript::fun(&Transform::scale), "scale");

         chai->add(chaiscript::user_type<EntityHandle>(), "EntityHandle");
         chai->add_global(chaiscript::var(ent), "entity");

         chai->add(chaiscript::fun(&ECS::GetComponent<Transform>, &ecs), "GetTransformComponent");

         try {
            chai->use(scriptComponent->scriptFile);
         } catch (const chaiscript::exception::eval_error &e) {
            std::cout << e.pretty_print() << '\n';
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
