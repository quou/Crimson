# Scripting API

Crimson uses ChaiScript, which follows C-style grammar and has a simple object model. It should be fairly easy to pick up for anyone who has experience with C++, C or JavaScript. To learn the basics of ChaiScript, please visit the examples on their [website](http://chaiscript.com/examples.html)

### Example script
```cpp
def OnBegin() {
   print("This is a script!");
}

def OnUpdate(float delta) {
   GetTransformComponent(entity).position.x += 1.0f * delta;
   GetTransformComponent(entity).rotation.y += 100.0f  * delta;
}
```

### Events
```cpp
def OnBegin() {
   // Called before the first frame
}

def OnUpdate(delta) {
   // Called every frame
}
```

### Per-Script Constants
All scripts have access to these, but they are different on each script
```cpp
EntityHandle entity; // The ID of the entity that this script is attached to
```

### ECS Functions
```cpp
TransformComponent* GetTransformComponent(EntityHandle ent);
ModelComponent* GetModelComponent(EntityHandle ent);
CameraComponent* GetCameraComponent(EntityHandle ent);
PointLightComponent* GetPointLightComponent(EntityHandle ent);
```

### ECS Components
```cpp
struct TransformComponent {
   vec3 position;
   vec3 rotation;
   vec3 scale;
};

struct ModelComponent {
   Crimson::Mesh mesh;
   Crimson::Texture texture;
   Crimson::Shader shader;
   Crimson::Material material;
};

struct CameraComponent {
   bool current;
   Crimson::Camera camera;
}

struct PointLightComponent {
   float constant;
   float linear;
   float quadratic;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};
```
