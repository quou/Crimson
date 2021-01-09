#shader vertex
#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv;

void main() {
	gl_Position = vec4(a_position, 1.0f);
}

#shader pixel
#version 330 core

out vec4 v_color;

void main() {
	v_color = vec4(1.0f);
}
