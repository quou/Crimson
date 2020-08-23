#include <SDL.h>
#include <glad/glad.h>

#include <iostream>

int main(int argc, char const *argv[]) {
   SDL_Init(SDL_INIT_EVERYTHING);
   SDL_Window* window = SDL_CreateWindow("Crimson", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);
   SDL_GLContext glContext = SDL_GL_CreateContext(window);

   if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
      std::cout << "Error initialising Glad" << '\n';
      return -1;
   }

   bool running = true;
   while (running) {
      SDL_Event e;
      while (SDL_PollEvent(&e)) {
         if (e.type == SDL_QUIT) {
            running = false;
         }
      }

      glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      SDL_GL_SwapWindow(window);
   }

   SDL_DestroyWindow(window);
   SDL_Quit();

   return 0;
}
