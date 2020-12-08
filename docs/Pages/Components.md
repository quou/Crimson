# Components

```cpp
struct TransformComponent {
	vec3 position;
	quat rotation;
	vec3 scale;
	std::string name;
	std::string tag;
	bool active;
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

struct DirectionalLightComponent {
	vec3 color;
	float intensity;
};

struct AmbientLightComponent {
	vec3 color;
	float intensity;
};

struct PointLightComponent {
	vec3 color;
	float intensity;

	float constant;
	float linear;
	float quadratic;
};

struct BoxColliderComponent {
	vec3 extents;
};

struct SphereColliderComponent {
	float radius;
}
```
