#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>

class GUI {
private:

public:
   GUI(){}
   GUI(SDL_Window* window, const SDL_GLContext glContext);
   void Init(SDL_Window* window, const SDL_GLContext glContext);
   void Render(SDL_Window* window);
   void Update(const SDL_Event& event);
   ~GUI();
};

#endif /* end of include guard: GUI_H */
