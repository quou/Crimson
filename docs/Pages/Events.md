# Events

All scripts that inherit from `CrimsonBehaviour` will have a number of specific functions that will be called upon certain events

## OnInit
Called before the first frame
```cpp
void OnInit();
```

## OnUpdate
Called every frame
```cpp
void OnUpdate(float delta);
```

## OnContact
Called when the entity collides with something. This event will not be called on entities that don't have a `PhysicsComponent`.
```cpp
void OnContact();
```
