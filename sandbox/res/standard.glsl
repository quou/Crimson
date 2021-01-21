#shader vertex
#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 v_uv;
out vec3 v_worldPos;
out vec3 v_normal;

void main() {
	v_uv = a_uv;
	v_normal = a_normal;
	v_worldPos = vec3(u_model * vec4(a_position, 1.0f));

	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0f);
}

#shader pixel
#version 330 core

in vec2 v_uv;
in vec3 v_worldPos;
in vec3 v_normal;
out vec4 v_color;

struct Material {
	vec3 albedo;
};

struct PointLight {
	vec3 position;
	vec3 color;
	float intensity;
};

uniform Material u_material = Material(vec3(1.0, 0.0, 0.0));

void main() {
	v_color = vec4(u_material.albedo, 1.0);
}
