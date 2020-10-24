#begin VERTEX

#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoords;

out vec2 v_texCoords;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
	gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0f);
	v_texCoords = vec2(a_texCoords.x, a_texCoords.y);
}

#end VERTEX

#begin FRAGMENT

#version 330 core

in vec2 v_texCoords;

uniform sampler2D u_albedo;

uniform vec3 u_color = vec3(1, 1, 1);

void main() {
	gl_FragColor = texture2D(u_albedo, v_texCoords) * vec4(u_color, 1.0f);
}

#end FRAGMENT
