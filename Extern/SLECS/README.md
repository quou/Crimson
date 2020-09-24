# SLECS
Super Lightweight Entity Component System - A < 500 line single-header ECS

## Prerequisites
 - *Before* including `SLECS.h`, you must `#define MAX_ENTITIES 100` and `#define MAX_COMPONENTS 32` in **one** source file.
 - You need a compiler with the C++ standard set to C++ 14 or above. It will fail to compile otherwise.

## Components
Components are simply C-style structs (ie. without methods)
```cpp
struct ExampleComponent {
   int x = 2;
   int y = 5;
   std::string someData = "Hello, world";
};
```

## Entity Management
The entity doesn't have a dedicated class, it is simply an id. Entities are stored in a vector by the main ECS class
```cpp
ECS ecsManager;

/* EntityHandle is an ID and a flag in the form of a 64-bit integer that points to an entity */
EntityHandle testEnt = ecsManager.CreateEntity();

/* Both AddComponent and GetComponent return a pointer to the component */
ecsManager.AddComponent<ExampleComponent>(testEnt)->x = 5;
ecsManager.GetComponent<ExampleComponent>(testEnt)->someData = "This is an example";
```

#### `T* ECS::AddComponent<T>(EntityHandle)`
Add a component to an entity, return a pointer to the new component

#### `T* ECS::GetComponent<T>(EntityHandle)`
Return a pointer to a component on an entity

#### `bool ECS::HasComponent<T>(EntityHandle)`
Return a bool indicating whether or not an entity has a component

#### `void ECS::RemoveComponent<T>(EntityHandle)`
Remove a component from an entity

#### `void ECS::DestroyEntity(EntityHandle)`
Destroy an entity and it's components


## Systems
The system is an iterator that iterates over entities with specific components
```cpp
/* This system loops over all the entities that have ExampleComponent and SomeTestComponent */
for (EntityHandle ent : System<ExampleComponent, SomeTestComponent>(ecsManager)) {

   /* GetEntityIndex returns an ID for the EntityHandle */
   std::cout << "\n" << "Entity " << GetEntityIndex(ent) << '\n';
   std::cout << "ExampleComponent someData: " << ecsManager.GetComponent<ExampleComponent>(ent)->someData << '\n';
   std::cout << "ExampleComponent x: " << ecsManager.GetComponent<ExampleComponent>(ent)->x << '\n';
   std::cout << "ExampleComponent y: " << ecsManager.GetComponent<ExampleComponent>(ent)->y << '\n';
}
```

## Complete Example
```cpp
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
   ecsManager.AddComponent<ExampleComponent>(testEnt);
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
```
