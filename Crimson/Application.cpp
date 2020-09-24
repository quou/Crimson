#include "Application.h"

#include <iostream>

#include <glad/glad.h>

#include "Scripting/ScriptWrapper.h"
#include "ComponentSystems/PrefabSystems.h"

namespace Crimson {
   Uint64 NOW = SDL_GetPerformanceCounter();
   Uint64 LAST = 0;

   Application::Application(const std::string& title) : m_title(title) {
      SDL_Init(SDL_INIT_EVERYTHING);

      m_display.Init(1366, 768, SDL_WINDOW_RESIZABLE, m_title.c_str());
      m_renderer.Init();

      m_sceneManager.Init();

      m_ecs.Init();

      m_ecs.RegisterComponent<CameraComponent>();
      m_ecs.RegisterComponent<ModelComponent>();
      m_ecs.RegisterComponent<Transform>();
      m_ecs.RegisterComponent<ScriptComponent>();
      m_ecs.RegisterComponent<PrefabInstancerComponent>();
      m_ecs.RegisterComponent<PointLight>();

      Signature camSig;
      camSig.set(m_ecs.GetComponentType<Transform>());
      camSig.set(m_ecs.GetComponentType<CameraComponent>());

      Signature graphicsSig;
      graphicsSig.set(m_ecs.GetComponentType<ModelComponent>());
      graphicsSig.set(m_ecs.GetComponentType<Transform>());

      Signature lightSig;
      lightSig.set(m_ecs.GetComponentType<Transform>());
      lightSig.set(m_ecs.GetComponentType<PointLight>());

      Signature transformSig;
      transformSig.set(m_ecs.GetComponentType<Transform>());

      Signature scriptSig;
      scriptSig.set(m_ecs.GetComponentType<ScriptComponent>());


      m_cameraSystem = m_ecs.RegisterSystem<CameraSystem>();
      m_ecs.SetSystemSignature<CameraSystem>(camSig);

      m_scriptSystem = m_ecs.RegisterSystem<ScriptSystem>();
      m_ecs.SetSystemSignature<ScriptSystem>(scriptSig);

      m_transformSystem = m_ecs.RegisterSystem<TransformSystem>();
      m_ecs.SetSystemSignature<TransformSystem>(transformSig);

      m_lightingSystem = m_ecs.RegisterSystem<LightingSystem>();
      m_ecs.SetSystemSignature<LightingSystem>(lightSig);

      m_graphicsSystem = m_ecs.RegisterSystem<GraphicsSystem>();
      m_ecs.SetSystemSignature<GraphicsSystem>(graphicsSig);
   }

   void Application::Init() {
      m_transformSystem->Init(m_ecs);

      OnBegin();
   }

   void Application::Update(float delta) {
      m_transformSystem->Update(m_ecs);
   }

   void Application::Render() {
      LAST = NOW;
      NOW = SDL_GetPerformanceCounter();
      m_deltaTime = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency() );
      m_deltaTime*=0.001f;

      m_renderer.Clear(0.0f, 0.0f, 0.0f, 0.0f);
      OnRender(m_deltaTime);
      m_display.Present();
   }

   void Application::Run() {
      Init();
      while (m_isRunning) {
         m_keyboard.Update();
         while (SDL_PollEvent(&m_event)) {
            if (m_eventCallback) {
               (*m_eventCallback) (m_event);
            }

            switch (m_event.type) {
            case SDL_QUIT:
               m_isRunning = false;
               break;
            case SDL_KEYDOWN:
               m_keyboard.KeyDownEvent(m_event);
               break;
            case SDL_KEYUP:
               m_keyboard.KeyUpEvent(m_event);
               break;
            case SDL_WINDOWEVENT:
               if (m_event.window.windowID == m_display.GetWindowID()) {
                  switch (m_event.window.event) {
                     case SDL_WINDOWEVENT_RESIZED:
                        m_display.ResizeWindow(m_event.window.data1, m_event.window.data2);
                        break;
                     default:
                        break;
                  }
               }
            default:
               break;
            }
         }

         Update(m_deltaTime);
         OnUpdate(m_deltaTime);
         Render();
      }
   }

   Application::~Application() {
      SDL_Quit();
      std::cout << "Quit" << '\n';
   }
}
