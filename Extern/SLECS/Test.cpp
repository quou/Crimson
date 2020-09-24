#include <iostream>


#define MAX_ENTITIES 100
#define MAX_COMPONENTS 32

#include "SLECS.h"



struct ExampleComponent {
   int value = 500;
};

struct AnotherExample {
   float x = 4.0f;
   float y = 5.0f;
};

int main(int argc, char const *argv[]) {
   ECS ecsManager;

   EntityHandle testEnt = ecsManager.CreateEntity();
   ecsManager.AddComponent<ExampleComponent>(testEnt, 25);
   ecsManager.AddComponent<AnotherExample>(testEnt);

   EntityHandle testEnt2 = ecsManager.CreateEntity();
   ecsManager.AddComponent<ExampleComponent>(testEnt2)->value = 10;
   ecsManager.AddComponent<AnotherExample>(testEnt2)->x = 100;
   ecsManager.GetComponent<AnotherExample>(testEnt2)->y = 2;

   for (EntityHandle ent : System<ExampleComponent, AnotherExample>(ecsManager)) {
      std::cout << "\n" << "Entity " << GetEntityIndex(ent) << '\n';
      std::cout << "ExampleComponent value: " << ecsManager.GetComponent<ExampleComponent>(ent)->value << '\n';
      std::cout << "AnotherExample x: " << ecsManager.GetComponent<AnotherExample>(ent)->x << '\n';
      std::cout << "AnotherExample y: " << ecsManager.GetComponent<AnotherExample>(ent)->y << '\n';
   }

   return 0;
}
