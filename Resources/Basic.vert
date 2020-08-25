#version 330 core

layout (location = 0) in vec4 position;

uniform mat4 proj;
uniform mat4 view = mat4(1.0f);
uniform mat4 modl = mat4(1.0f);

void main() {
   gl_Position = proj * position;
}
