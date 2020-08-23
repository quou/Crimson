#include <SDL.h>

#include <iostream>

int main(int argc, char const *argv[]) {
   SDL_Init(SDL_INIT_EVERYTHING);
   SDL_Window* window = SDL_CreateWindow("Crimson", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);

   bool running = true;
   while (running) {
      SDL_Event e;
      while (SDL_PollEvent(&e)) {
         if (e.type == SDL_QUIT) {
            running = false;
         }
      }
   }

   SDL_DestroyWindow(window);
   SDL_Quit();

   return 0;
}
