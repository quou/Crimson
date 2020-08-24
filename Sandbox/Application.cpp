#include <Crimson.h>

class Application : public Crimson::Application {
private:
public:
   void OnBegin() override {
      Crimson::Matrix4 pos = Crimson::Matrix4::Translate(Crimson::Vector3(2, 3, 4));
      Crimson::Vector4 col = pos.columns[3];
      std::cout << col << '\n';
   }

   void OnUpdate(float delta) override {

   }
};

int main(int argc, char const *argv[]) {
   Application app;
   app.Run();

   return 0;
}
