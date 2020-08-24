#include <Crimson.h>

class Application : public Crimson::Application {
private:
public:
   void OnBegin() override {
      Crimson::Vector3 testVec(1, 3, 1);
      Crimson::Vector3 testVec2(1, 0, 1);

      if (testVec != testVec2) {
         std::cout << "hi" << '\n';
      }
   }

   void OnUpdate(float delta) override {

   }
};

int main(int argc, char const *argv[]) {
   Application app;
   app.Run();

   return 0;
}
