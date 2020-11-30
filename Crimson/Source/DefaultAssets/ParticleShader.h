#pragma once

static const char* ParticleShader = R"(
#begin VERTEX

#version 330 core

#define MAX_PARTICLES 500

layout (location = 0) in vec4 a_vertex;

uniform mat4 u_projection;
uniform mat4 u_view;

uniform mat4 u_transforms[MAX_PARTICLES];
uniform int u_numParticles;

void main() {
	gl_Position = u_projection * u_view * u_transforms[gl_InstanceID] * vec4(a_vertex.xy, 0.0, 1.0);
}

#end VERTEX

#begin FRAGMENT

#version 330 core

out vec4 v_fragColor;

void main() {
	v_fragColor = vec4(1, 1, 1, 1);
}

#end FRAGMENT
)";
