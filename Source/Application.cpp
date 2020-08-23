#include <SDL.h>
#include <glad/glad.h>

#include <iostream>

#include "Graphics/Display.h"

int main(int argc, char const *argv[]) {
   SDL_Init(SDL_INIT_EVERYTHING);

   Display display(800, 600, 0, "Crimson Game Engine");

   bool running = true;
   while (running) {
      SDL_Event e;
      while (SDL_PollEvent(&e)) {
         if (e.type == SDL_QUIT) {
            running = false;
         }
      }

      display.Clear(0.1f, 0.3f, 0.5f, 1.0f);
      display.Present();
   }

   return 0;
}
