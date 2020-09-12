#include "ScriptWrapper.h"

#include "ScriptStandardFunctions.h"
#include "ComponentSystems/Transform.h"

#include <glm/glm.hpp>

namespace Crimson {
   namespace Scripting {
      void Execute(ScriptComponent* scriptComponent) {
         chaiscript::ChaiScript* chai = &scriptComponent->chai;

         try {
            chai->use(scriptComponent->scriptFile);
         } catch (const chaiscript::exception::eval_error &e) {
            std::cout << "Error\n" << e.pretty_print() << '\n';
         }
      }

      void CallUpdateFunction(ScriptComponent* scriptComponent, float delta) {
         try {
            auto p = scriptComponent->chai.eval<std::function<void (float)>>("OnUpdate");
            p(delta);
         } catch (const chaiscript::exception::eval_error &e) {
            std::cout << "Error\n" << e.pretty_print() << '\n';
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
            std::cout << "Error\n" << e.pretty_print() << '\n';
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
