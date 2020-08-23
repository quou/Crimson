#include <Crimson.h>

class Application : public Crimson::Application {
private:
public:
   void OnUpdate(float delta) override {

   }
};

int main(int argc, char const *argv[]) {
   Application app;
   app.Run();

   return 0;
}
