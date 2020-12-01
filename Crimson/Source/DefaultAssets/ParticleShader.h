#pragma once

static const char* ParticleShader = R"(
#begin VERTEX

#version 330 core

#define MAX_PARTICLES 1000

layout (location = 0) in vec4 a_vertex;

out vec4 v_vertex;

uniform mat4 u_projection;
uniform mat4 u_view;

uniform mat4 u_transforms[MAX_PARTICLES];
uniform int u_numParticles;

void main() {
	v_vertex = a_vertex;

	gl_Position = u_projection * u_view * u_transforms[gl_InstanceID] * vec4(a_vertex.xy, 0.0, 1.0);
}

#end VERTEX

#begin FRAGMENT

#version 330 core

out vec4 v_fragColor;

in vec4 v_vertex;

uniform sampler2D u_albedo;

struct Material {
	vec3 color;
};

uniform Material u_material = Material(vec3(1));

void main() {
	vec4 fragColor = texture(u_albedo, v_vertex.zw) * vec4(u_material.color, 1.0);

	if(fragColor.a < 0.1) {
		discard;
	}
	v_fragColor = fragColor;
}

#end FRAGMENT
)";
