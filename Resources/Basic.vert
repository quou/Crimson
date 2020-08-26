#version 330 core

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;

out vec2 v_texCoord;

uniform mat4 proj;
uniform mat4 view = mat4(1.0f);
uniform mat4 modl = mat4(1.0f);

void main() {
   gl_Position = proj * view * modl * vec4(position, 1.0);
   v_texCoord = texCoord;
}
