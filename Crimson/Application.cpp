#include "Application.h"

#include <iostream>

#include <glad/glad.h>

namespace Crimson {
   void Application::Init() {
      SDL_Init(SDL_INIT_EVERYTHING);

      m_display.Init(800, 600, 0, "Test Application");
      m_renderer.Init();
      OnBegin();
   }

   void Application::Update(float delta) {

   }

   void Application::Render() {
      m_renderer.Clear(0.1f, 0.3f, 0.5f, 1.0f);
      OnRender(1.0f);
      m_display.Present();
   }

   void Application::Run() {
      Init();
      while (m_isRunning) {
         SDL_Event e;
         while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
               m_isRunning = false;
               break;
            default:
               break;
            }
         }

         Update(1.0f);
         OnUpdate(1.0f);
         Render();
      }
   }

   Application::~Application() {
      SDL_Quit();
      std::cout << "Quit" << '\n';
   }
}
