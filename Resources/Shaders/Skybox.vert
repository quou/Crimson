#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec3 v_texCoord;

uniform mat4 view = mat4(1.0f);

void main() {
   v_texCoord = position;
   gl_Position = view * vec4(position, 1.0);
}
