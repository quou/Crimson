#pragma once

namespace Crimson {
	namespace ErrorAssets {
		static const char* ErrorShaderCode = R"(
#shader vertex
#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
}

#shader pixel
#version 330 core

out vec4 v_color;

void main() {

	v_color = vec4(1.0, 0.0, 1.0, 1.0);
}
		)";
	}
}