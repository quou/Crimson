#include <SDL.h>
#include <glad/glad.h>

#include <iostream>

#include <Crimson.h>

class Application : public Crimson::Application {
private:
public:
   void OnUpdate(float delta) override {
      std::cout << "Hello" << '\n';
   }
};

int main(int argc, char const *argv[]) {
   Application app;
   app.Run();

   return 0;
}
