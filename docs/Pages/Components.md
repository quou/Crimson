# Components

```cpp
struct TransformComponent {
	vec3 position;
	vec3 rotation;
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

	void ApplyForce(const vec3& force);
	void ApplyForceAtPosition(const glm::vec3& position, const vec3& force);
	void ApplyTorque(const vec3& torque);
};
```
