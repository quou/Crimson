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

## OnPhysicsUpdate
Called every physics update
```cpp
void OnPhysicsUpdate(float delta);
```

## OnContact
Called when the entity collides with something. This event will not be called on entities that don't have a `PhysicsComponent`.
```cpp
// Called when the entity begins colliding with something
void OnContactEnter(Entity other);

// Called every frame until the entity stops colliding
void OnContactStay(Entity other);

// Called when the entity stops colliding
void OnContactExit(Entity other);
```
