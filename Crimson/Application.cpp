#include "Application.h"

#include <iostream>

#include <glad/glad.h>

namespace Crimson {
   Uint64 NOW = SDL_GetPerformanceCounter();
   Uint64 LAST = 0;

   Application::Application() {
      SDL_Init(SDL_INIT_EVERYTHING);

      m_display.Init(1366, 768, SDL_WINDOW_RESIZABLE, "Test Application");
      m_renderer.Init();
   }

   void Application::Init() {
      OnBegin();
   }

   void Application::Update(float delta) {

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

         Update(1.0f);
         OnUpdate(m_deltaTime);
         Render();
      }
   }

   Application::~Application() {
      SDL_Quit();
      std::cout << "Quit" << '\n';
   }
}
