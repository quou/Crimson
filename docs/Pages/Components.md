# Components

```cpp
struct TransformComponent {
	vec3 position;
	quat rotation;
	vec3 scale;
	std::string name;
	std::string tag;
};

struct PhysicsComponent {
	bool useGravity;
	float mass;
	float friction;
	float bounciness;
	bool isKinematic;
	vec3 cog;

	Rigidbody& context;

	void ApplyForce(const vec3 &in force);
	void ApplyForceAtPosition(const glm::vec3 &in position, const vec3 &in force);
	void ApplyTorque(const vec3 &in torque);
};

struct MaterialComponent {
	string resource;
};

struct MeshFilterComponent {
	string resource;
};
```
