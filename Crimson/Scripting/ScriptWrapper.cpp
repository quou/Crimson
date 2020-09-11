#include "ScriptWrapper.h"

#include "ScriptStandardFunctions.h"
#include <cassert>

#include <scriptbuilder/scriptbuilder.h>
#include <scriptstdstring/scriptstdstring.h>

namespace Crimson {
   namespace Scripting {
      asIScriptEngine* CreateEngine() {
         return asCreateScriptEngine();
      }

      void Shutdown(asIScriptEngine* engine) {
         engine->ShutDownAndRelease();
      }

      void SetupMessageSystem(asIScriptEngine* engine) {
         int r = engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL); assert( r >= 0 );
      }

      void RegisterFunctions(asIScriptEngine* engine) {
         RegisterStdString(engine);
         int r = engine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(print), asCALL_CDECL); assert( r >= 0 );
      }

      void InitModule(ScriptComponent* scriptComponent, asIScriptEngine* engine) {
         scriptComponent->compiled = false;

         if (scriptComponent->module) {
            engine->DiscardModule(scriptComponent->scriptFile.c_str());
            //scriptComponent->module->Discard();
         }

         if (scriptComponent->context) {
            scriptComponent->context->Release();
         }

         scriptComponent->module = nullptr;
         scriptComponent->context = nullptr;

         CScriptBuilder builder;
         int r = builder.StartNewModule(engine, scriptComponent->scriptFile.c_str());
         if( r < 0 )
         {
            // If the code fails here it is usually because there
            // is no more memory to allocate the module
            printf("Unrecoverable error while starting a new module.\n");
            return;
         }
         r = builder.AddSectionFromFile(scriptComponent->scriptFile.c_str());
         if( r < 0 )
         {
            // The builder wasn't able to load the file. Maybe the file
            // has been removed, or the wrong name was given, or some
            // preprocessing commands are incorrectly written.
            printf("Couldn't load script file: %s\n", scriptComponent->scriptFile.c_str());
            return;
         }
         r = builder.BuildModule();
         if( r < 0 )
         {
            // An error occurred.
            return;
         }
         scriptComponent->compiled = true;

         scriptComponent->module = engine->GetModule(scriptComponent->scriptFile.c_str());

         // Create our context, prepare it, and then execute
         scriptComponent->context = engine->CreateContext();
      }

      void DeinitModule(ScriptComponent* scriptComponent, asIScriptEngine* engine) {
         if (scriptComponent->context) {
            scriptComponent->context->Release();
         }
      }

      void CallFunction(std::string prototype, ScriptComponent* scriptComponent, asIScriptEngine* engine) {
         if (!scriptComponent->compiled) {
            return;
         }

         asIScriptFunction *func = scriptComponent->module->GetFunctionByDecl(prototype.c_str());
         if(func == 0) {
            return;
         }

         scriptComponent->context->Prepare(func);
         int r = scriptComponent->context->Execute();
         if( r != asEXECUTION_FINISHED )
         {
            // The execution didn't complete as expected. Determine what happened.
            if( r == asEXECUTION_EXCEPTION )
            {
               // An exception occurred, let the script writer know what happened so it can be corrected.
               printf("An exception '%s' occurred.\n", scriptComponent->context->GetExceptionString());
            }
         }
         scriptComponent->context->Unprepare();
      }

      void CallOnUpdate(ScriptComponent* scriptComponent, asIScriptEngine* engine, float delta) {
         if (!scriptComponent->compiled) {
            return;
         }

         asIScriptFunction *func = scriptComponent->module->GetFunctionByDecl("void OnUpdate(float)");
         if(func == 0) {
            return;
         }

         scriptComponent->context->Prepare(func);
         scriptComponent->context->SetArgFloat(0, delta);
         int r = scriptComponent->context->Execute();
         if( r != asEXECUTION_FINISHED )
         {
            // The execution didn't complete as expected. Determine what happened.
            if( r == asEXECUTION_EXCEPTION )
            {
               // An exception occurred, let the script writer know what happened so it can be corrected.
               printf("An exception '%s' occurred.\n", scriptComponent->context->GetExceptionString());
            }
         }
         scriptComponent->context->Unprepare();
      }
   }

}
